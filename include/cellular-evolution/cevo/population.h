#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/utilities/random.h"
#include "cellular-evolution/utilities/evo_utils.h"
#include "cellular-evolution/cevo/pop_config.h"
#include "cellular-evolution/utilities/bitmap.hpp"
#include <thread>
#include <mutex>
#include<queue>
#include<vector>
#include<thread>


enum ThrAction {
    INIT, RANDOMIZE, NEXTGEN
};

struct ThrTask {

    ThrAction act;
    int from, to;
};

template<class Individual, class IndData>
class Population {
public:


    // Function that evaluate queue of commands, consumer thread
    void Consume() {
        ThrTask val;
        Random rnd;

        IndData thread_data(m_indData);

        int inds[13];

        while (m_taskList.size() != 0) {
            {
                std::lock_guard<std::mutex> l(task_list_mutex_);
                // Get value
                if (m_taskList.size() == 0) {
                    break;
                }
                val = m_taskList.front();
                m_taskList.pop();
            }


            switch (val.act) {
                // Initialize population
                case INIT:
                    for (int i = val.from; i < val.to; i++) {
                        m_currGenInds[i] = m_templateInd;
                        m_nextGenInds[i] = m_templateInd;
                    }
                    break;

                    // Randomize each initialized individual
                case RANDOMIZE:
                    for (int i = val.from; i < val.to; i++) {
                        m_currGenInds[i].randomize(thread_data, rnd);
                        m_currGenInds[i].countFitness(thread_data);
                        if (m_drawpop) {
                            m_currGenInds[i].countColor(thread_data);
                        }
                    }

                    break;

                    // Next generation - copy the best to new population and mutate them.
                case NEXTGEN:
                    int first_ind = -1;
                    int second_ind = -1;

                    for (int i = val.from; i < val.to; i++) {
                        if (rnd.nextFloat() < m_crossrate) {

                            // int first_ind=roulette(prepareD13(i / m_popHeight, i % m_popHeight,inds),13,rnd);
                            // int second_ind=roulette(prepareD13(i / m_popHeight, i % m_popHeight,inds),13,rnd);

                            //int first_ind=tournament(NRands(prepareL5(i / m_popHeight, i % m_popHeight,inds),5,4,rnd),4);
                            //int second_ind=tournament(NRands(prepareL5(i / m_popHeight, i % m_popHeight,inds),5,4,rnd),4);

                            dualTournament(prepareL5(i / m_popHeight, i % m_popHeight, inds), 5, first_ind, second_ind);

                            if (second_ind != -1) {
                                m_currGenInds[first_ind].crossoverTo(m_currGenInds[second_ind], m_nextGenInds[i],
                                                                     rnd, thread_data);
                            } else {
                                m_currGenInds[first_ind].mutateTo(rnd.nextFloat() * m_mutamount, m_mutprob,
                                                                  m_nextGenInds[i],
                                                                  rnd, thread_data);
                            }
                        } else {
                            int first_ind = tournament(prepareL5(i / m_popHeight, i % m_popHeight, inds), 5);
                            //int first_ind=tournament(NRands(prepareL5(i / m_popHeight, i % m_popHeight,inds),5,4,rnd),4);
                            m_currGenInds[first_ind].mutateTo(rnd.nextFloat() * m_mutamount,
                                                              m_mutprob, m_nextGenInds[i],
                                                              rnd, thread_data);
                        }

                        m_nextGenInds[i].countFitness(thread_data); // TRAIN AND TEST SPLIT
                        if (m_drawpop) {
                            m_nextGenInds[i].countColor(thread_data);
                        }
                    }
                    break;
            }

        }
    }

    Population(Individual const &srcInd, IndData const &data, PopConfig const &cfg) {
        m_indData = data;

        m_popWidth = cfg.getInt("xpopsize")[0];
        m_popHeight = cfg.getInt("ypopsize")[0];
        m_inds_cnt = m_popWidth * m_popHeight;
        m_number_of_threads_ = cfg.getThreads();
        m_drawpop = cfg.getBool("drawpop")[0];
        m_crossrate = cfg.getFloat("crossrate")[0];
        m_mutamount = cfg.getFloat("mutamount")[0];
        m_mutprob = cfg.getFloat("mutprob")[0];

        if (cfg.is_present("cluster_size"))
        {
            cluster_size = cfg.getInt("cluster_size")[0];
        }


        image_.init(m_popWidth, m_popHeight);

        m_templateInd = srcInd;


        m_currGenInds = new Individual[m_popWidth * m_popHeight];
        m_nextGenInds = new Individual[m_popWidth * m_popHeight];

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * cluster_size);

        int from = 0;
        while (from + step < m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = from + step;
            tsk.act = ThrAction::INIT;
            m_taskList.emplace(std::move(tsk));
            from += step;
        }

        if (from != m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = m_inds_cnt;
            tsk.act = ThrAction::INIT;
            m_taskList.emplace(std::move(tsk));
        }

        m_threads.clear();
        for (int i = 0; i < m_number_of_threads_; i++) {
            m_threads.emplace_back(std::thread(&Population<Individual, IndData>::Consume, this));
        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }

        m_gensCount = 0;
    }


    ~Population() {
        if (m_currGenInds != nullptr) {
            delete[]m_currGenInds;
        }
        if (m_nextGenInds != nullptr) {
            delete[]m_nextGenInds;
        }
    }

    void nextGen() {

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * cluster_size);

        int from = 0;
        while (from + step < m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = from + step;
            tsk.act = ThrAction::NEXTGEN;
            m_taskList.emplace(std::move(tsk));
            from += step;
        }

        if (from != m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = m_inds_cnt;
            tsk.act = ThrAction::NEXTGEN;
            m_taskList.emplace(std::move(tsk));
        }

        m_threads.clear();
        for (int i = 0; i < m_number_of_threads_; i++) {
            m_threads.emplace_back(std::thread(&Population<Individual, IndData>::Consume, this));
        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }

        Individual *tmp = m_currGenInds;
        m_currGenInds = m_nextGenInds;
        m_nextGenInds = tmp;

        m_gensCount++;
    }

    void Randomize() {

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * cluster_size);

        int from = 0;
        while (from + step < m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = from + step;
            tsk.act = ThrAction::RANDOMIZE;
            m_taskList.emplace(std::move(tsk));
            from += step;
        }

        if (from != m_inds_cnt) {
            ThrTask tsk;
            tsk.from = from;
            tsk.to = m_inds_cnt;
            tsk.act = ThrAction::RANDOMIZE;
            m_taskList.emplace(std::move(tsk));
        }

        m_threads.clear();
        for (int i = 0; i < m_number_of_threads_; i++) {
            m_threads.emplace_back(std::thread(&Population<Individual, IndData>::Consume, this));
        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }
    }

    Individual getBest() {
        int best = 0;

        for (int i = 1; i < m_inds_cnt; i++) {

            if (m_currGenInds[best].fitness < m_currGenInds[i].fitness) {
                best = i;
            }
        }

        return m_currGenInds[best];
    }

    Individual getWorst() {
        int worst = 0;

        for (int i = 1; i < m_inds_cnt; i++) {

            if (m_currGenInds[worst].fitness > m_currGenInds[i].fitness) {
                worst = i;
            }
        }

        return m_currGenInds[worst];
    }

    float avgFitness() {
        float avgFit = 0;

        for (int i = 0; i < m_inds_cnt; i++) {
            avgFit += m_currGenInds[i].getFitness();
        }
        return avgFit / (m_popWidth * m_popHeight);
    }

    void paintPop(std::string imgName) {
        int red, green, blue;
        int l, a, b;

        Individual best = getBest();
        Individual worst = getWorst();
        float max = best.getFitness();
        float min = worst.getFitness();

        float diff = max - min;

        float max_x = m_currGenInds[0].colX;
        float min_x = m_currGenInds[0].colX;

        float max_y = m_currGenInds[0].colY;
        float min_y = m_currGenInds[0].colY;

        for (int i = 1; i < m_inds_cnt; i++) {
            if (m_currGenInds[i].colX > max_x)max_x = m_currGenInds[i].colX;
            if (m_currGenInds[i].colY > max_y)max_y = m_currGenInds[i].colY;
            if (m_currGenInds[i].colX < min_x)min_x = m_currGenInds[i].colX;
            if (m_currGenInds[i].colY < min_y)min_y = m_currGenInds[i].colY;
        }
        float diff_x = max_x - min_x;
        float diff_y = max_y - min_y;

        //Bitmap image (m_popWidth, m_popHeight);

        for (int i = 0; i < m_inds_cnt; i++) {

            int xx = i / m_popHeight;
            int yy = i % m_popHeight;

            int pix_intensity = static_cast<int>(((m_currGenInds[i].getFitness() - min) * 100.0) / diff);
            int pix_color_x = static_cast<int> (((m_currGenInds[i].colX - min_x) * 100.0) / diff_x);
            int pix_color_y = static_cast<int> (((m_currGenInds[i].colY - min_y) * 100.0) / diff_y);

            l = pix_intensity;
            a = pix_color_x - 50;
            b = pix_color_y - 50;

            LABtoRGB(red, green, blue, l, a, b);

            image_.setPixel(xx, yy, red, green, blue);
        }

        image_.Write(imgName);
    }

    /////////////////////////////////
    // Selection related functions //
    /////////////////////////////////
    int TranslateCoords(int x, int y) {
        if (x < 0) {
            x = x + m_popWidth;
        } else if (x >= m_popWidth) {
            x = x - m_popWidth;
        }

        if (y < 0) {
            y = y + m_popHeight;
        } else if (y >= m_popHeight) {
            y = y - m_popHeight;
        }

        return x * m_popHeight + y;
    }

    int *prepareL5(int x, int y, int *inds) {
        inds[0] = TranslateCoords(x, y);
        inds[1] = TranslateCoords(x - 1, y);
        inds[2] = TranslateCoords(x + 1, y);
        inds[3] = TranslateCoords(x, y - 1);
        inds[4] = TranslateCoords(x, y + 1);
        return inds;
    }


    int *prepareD13(int x, int y, int *inds) {
        inds[0] = TranslateCoords(x, y);
        inds[1] = TranslateCoords(x - 1, y);
        inds[2] = TranslateCoords(x + 1, y);
        inds[3] = TranslateCoords(x, y - 1);
        inds[4] = TranslateCoords(x, y + 1);
        inds[5] = TranslateCoords(x - 2, y);
        inds[6] = TranslateCoords(x + 2, y);
        inds[7] = TranslateCoords(x, y - 2);
        inds[8] = TranslateCoords(x, y + 2);
        inds[9] = TranslateCoords(x - 1, y - 1);
        inds[10] = TranslateCoords(x + 1, y + 1);
        inds[11] = TranslateCoords(x + 1, y - 1);
        inds[12] = TranslateCoords(x - 1, y + 1);

        return inds;
    }


    int *NRands(int *inds, int size, int n, Random &rnd) {
        int next;
        for (int i = 0; i < n; i++) {
            next = rnd.nextInt(i, size - 1);

            int tmp = inds[i];
            inds[i] = inds[next];
            inds[next] = tmp;
        }
        return inds;
    }


    int tournament(int *inds, int size) {
        //System.out.println(x+", "+y);
        int bestInd = inds[0];

        for (int i = 1; i < size; i++) {
            if (m_currGenInds[inds[i]].fitness > m_currGenInds[bestInd].fitness) {
                bestInd = inds[i];
            }
        }

        return bestInd;
    }


    int roulette(int *inds, int size, Random &rnd) {
        int bestInd = inds[size - 1];

        float fit_sum = 0;
        for (int i = 0; i < size; i++) {
            fit_sum += 1.0 / m_currGenInds[inds[i]].fitness;
        }

        float val = rnd.nextFloat(0.0, 1.0);

        float prob_sum = 0;
        for (int i = 0; i < size - 1; i++) {
            prob_sum += (1.0 / m_currGenInds[inds[i]].fitness) / fit_sum;
            if (prob_sum >= val) {
                bestInd = inds[i];
                break;
            }
        }

        return bestInd;
    }

    void dualTournament(int *inds, int size, int &ind_1, int &ind_2) {
        ind_1 = inds[0];
        ind_2 = -1;

        for (int i = 1; i < size; i++) {

            int another_ind = inds[i];
            if (m_currGenInds[another_ind].fitness > m_currGenInds[ind_1].fitness) {
                ind_2 = ind_1;
                ind_1 = another_ind;
            } else if (m_currGenInds[another_ind].fitness < m_currGenInds[ind_1].fitness) {

                if (ind_2 != -1) {
                    if (m_currGenInds[another_ind].fitness > m_currGenInds[ind_2].fitness) {
                        ind_2 = another_ind;
                    }
                } else {
                    ind_2 = another_ind;
                }
            }
        }

    }

    int getGen() {
        return m_gensCount;
    }


    void ClearTasks() {
        std::queue<ThrTask> empty;
        std::swap(m_taskList, empty);
    }

    Individual m_templateInd;
    Individual *m_currGenInds = nullptr;
    Individual *m_nextGenInds = nullptr;
    int m_inds_cnt;
    int m_popWidth, m_popHeight;

    int m_gensCount;
    bool m_drawpop;
    float m_crossrate;
    float m_mutamount;
    float m_mutprob;

    IndData m_indData;

    Bitmap image_;


    std::vector<std::thread> m_threads;
    int m_number_of_threads_;

    std::queue<ThrTask> m_taskList;
    std::mutex task_list_mutex_;

    int cluster_size = 256;
};