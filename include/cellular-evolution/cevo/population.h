#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/utilities/random.h"
#include "cellular-evolution/utilities/evo_utils.h"
//#include "cellular-evolution/cevo/individual.h"
#include "cellular-evolution/cevo/ind_data.h"

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

template<typename Individual>
class Population {
public:


    // Function that evaluate queue of commands, consumer thread
    void consume() {
        ThrTask val;
        Random rnd;

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
                        m_currGenInds[i] = m_templateInd->UnsafeClone();
                        m_nextGenInds[i] = m_templateInd->makeBlank(); // Temporary can be blank
                    }
                    break;

                    // Randomize each initialized individual
                case RANDOMIZE:
                    for (int i = val.from; i < val.to; i++) {
                        m_currGenInds[i]->randomize(m_indData, rnd);
                        m_currGenInds[i]->countFitness(m_indData);
                        if (m_cfg->drawpop) {
                            m_currGenInds[i]->countColor();
                        }
                    }

                    break;

                    // Next generation - copy the best to new population and mutate them.
                case NEXTGEN:
                    Individual *first_ind = NULL;
                    Individual *second_ind = NULL;

                    for (int i = val.from; i < val.to; i++) {
                        if (rnd.nextFloat() < m_cfg->crossrate) {
                            dualTournamentL5(i / m_popHeight, i % m_popHeight, first_ind, second_ind);

                            if (second_ind != NULL) {
                                first_ind->crossoverTo(second_ind, m_nextGenInds[i], rnd);
                            } else {
                                first_ind->mutateTo(rnd.nextFloat() * m_cfg->mutamount, m_cfg->mutprob,
                                                    m_nextGenInds[i],
                                                    rnd);
                            }
                        } else {
                            tournamentL5(i / m_popHeight, i % m_popHeight)->mutateTo(rnd.nextFloat() * m_cfg->mutamount,
                                                                                     m_cfg->mutprob, m_nextGenInds[i],
                                                                                     rnd);
                        }

                        m_nextGenInds[i]->countFitness(m_indData); // TRAIN AND TEST SPLIT
                        if (m_cfg->drawpop) {
                            m_nextGenInds[i]->countColor();
                        }
                    }
                    break;
            }

        }
    }

    Population(std::shared_ptr<Individual> &srcInd, std::shared_ptr<IndData> &data, std::shared_ptr<PopConfig> &cfg) {
        m_cfg = cfg;
        m_indData = data;

        m_popWidth = cfg->xpopsize;
        m_popHeight = cfg->ypopsize;
        m_inds_cnt = m_popWidth * m_popHeight;
        m_number_of_threads_ = cfg->threads;

        image_ = std::make_shared<Bitmap>(m_popWidth, m_popHeight);

        m_templateInd = srcInd;


        m_currGenInds = new Individual *[m_popWidth * m_popHeight];
        m_nextGenInds = new Individual *[m_popWidth * m_popHeight];

        srcInd->countFitness(data);
        if (cfg->drawpop) {
            srcInd->countColor();
        }

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * 128);

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
            m_threads.emplace_back(std::thread(&Population<Individual>::consume, this));


            // Start both threads
            //m_threads[i].start();

        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }

        m_gensCount = 0;
        //rnd = new Random();
    }

    void nextGen() {

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * 128);

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
            m_threads.emplace_back(std::thread(&Population<Individual>::consume, this));
        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }

        Individual **tmp = m_currGenInds;
        m_currGenInds = m_nextGenInds;
        m_nextGenInds = tmp;

        m_gensCount++;
    }

    void Randomize() {

        ClearTasks();
        int step = m_inds_cnt / (m_number_of_threads_ * 128);

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
            m_threads.emplace_back(std::thread(&Population<Individual>::consume, this));
        }

        for (int i = 0; i < m_number_of_threads_; i++) {
            // t1 finishes before t2
            m_threads[i].join();
        }
    }

    Individual *getBest() {
        Individual *best = m_currGenInds[0];

        for (int i = 1; i < m_inds_cnt; i++) {

            if (best->fitness < m_currGenInds[i]->fitness) {
                best = m_currGenInds[i];
            }
        }

        return best;
    }

    Individual *getWorst() {
        Individual *worst = m_currGenInds[0];

        for (int i = 0; i < m_inds_cnt; i++) {

            if (worst->fitness > m_currGenInds[i]->fitness) {
                worst = m_currGenInds[i];
            }
        }

        return worst;
    }

    float avgFitness() {
        float avgFit = 0;

        for (int i = 0; i < m_inds_cnt; i++) {
            avgFit += m_currGenInds[i]->fitness;
        }
        return avgFit / (m_cfg->xpopsize * m_cfg->ypopsize);
    }

    void paintPop(std::string imgName) {

        //System.out.println("PAINTIN");
        int red, green, blue;
        int l, a, b;


        Individual *best = getBest();
        Individual *worst = getWorst();
        float max = best->fitness;
        float min = worst->fitness;

        /*float max = 255;
        float min = 0;*/
        float diff = max - min;


        float max_x = m_currGenInds[0]->colX;
        float min_x = m_currGenInds[0]->colX;

        float max_y = m_currGenInds[0]->colY;
        float min_y = m_currGenInds[0]->colY;

        for (int i = 1; i < m_inds_cnt; i++) {
            if (m_currGenInds[i]->colX > max_x)max_x = m_currGenInds[i]->colX;
            if (m_currGenInds[i]->colY > max_y)max_y = m_currGenInds[i]->colY;
            if (m_currGenInds[i]->colX < min_x)min_x = m_currGenInds[i]->colX;
            if (m_currGenInds[i]->colY < min_y)min_y = m_currGenInds[i]->colY;
        }
        float diff_x = max_x - min_x;
        float diff_y = max_y - min_y;

        //Bitmap image (m_popWidth, m_popHeight);

        for (int i = 0; i < m_inds_cnt; i++) {

            int xx = i / m_popHeight;
            int yy = i % m_popHeight;

            int pix_intensity = static_cast<int>(((m_currGenInds[i]->fitness - min) * 100.0) / diff);
            int pix_color_x = static_cast<int> (((m_currGenInds[i]->colX - min_x) * 100.0) / diff_x);
            int pix_color_y = static_cast<int> (((m_currGenInds[i]->colY - min_y) * 100.0) / diff_y);

            l = pix_intensity;
            a = pix_color_x - 50;
            b = pix_color_y - 50;

            LABtoRGB(red, green, blue, l, a, b);

            image_->SetPixel(xx, yy, red, green, blue);
        }

        image_->Write(imgName);
    }

    Individual *tournamentL5(int x, int y) {
        //System.out.println(x+", "+y);
        Individual *bestInd = m_currGenInds[x * m_popHeight + y];
        //System.out.println(xpopsize+",,"+ypopsize);
        int xa, ya;

        xa = x - 1;
        if (xa < 0) {
            xa += m_cfg->xpopsize;
        }

        if (m_currGenInds[xa * m_popHeight + y]->fitness > bestInd->fitness) {
            bestInd = m_currGenInds[xa * m_popHeight + y];
        }

        xa = x + 1;

        if (xa >= m_cfg->xpopsize) {
            xa -= m_cfg->xpopsize;
        }

        if (m_currGenInds[xa * m_popHeight + y]->fitness > bestInd->fitness) {
            bestInd = m_currGenInds[xa * m_popHeight + y];
        }

        ya = y - 1;

        if (ya < 0) {
            ya += m_cfg->ypopsize;
        }

        if (m_currGenInds[x * m_popHeight + ya]->fitness > bestInd->fitness) {
            bestInd = m_currGenInds[x * m_popHeight + ya];
        }

        ya = y + 1;

        if (ya >= m_cfg->ypopsize) {
            ya -= m_cfg->ypopsize;
        }

        if (m_currGenInds[x * m_popHeight + ya]->fitness > bestInd->fitness) {
            bestInd = m_currGenInds[x * m_popHeight + ya];
        }

        return bestInd;
    }

    void dualTournamentL5(int x, int y, Individual *&ind_1, Individual *&ind_2) {
        ind_1 = m_currGenInds[x * m_popHeight + y];
        ind_2 = NULL;

        //System.out.println(xpopsize+",,"+ypopsize);
        int xa, ya;

        xa = x - 1;
        if (xa < 0) {
            xa += m_cfg->xpopsize;
        }

        if (m_currGenInds[xa * m_popHeight + y]->fitness > ind_1->fitness) {
            ind_2 = ind_1;
            ind_1 = m_currGenInds[xa * m_popHeight + y];
        } else if (m_currGenInds[xa * m_popHeight + y]->fitness < ind_1->fitness) {

            if (ind_2 != NULL) {
                if (m_currGenInds[xa * m_popHeight + y]->fitness > ind_2->fitness) {
                    ind_2 = m_currGenInds[xa * m_popHeight + y];
                }
            } else {
                ind_2 = m_currGenInds[xa * m_popHeight + y];
            }
        }

        xa = x + 1;

        if (xa >= m_cfg->xpopsize) {
            xa -= m_cfg->xpopsize;
        }

        if (m_currGenInds[xa * m_popHeight + y]->fitness > ind_1->fitness) {
            ind_2 = ind_1;
            ind_1 = m_currGenInds[xa * m_popHeight + y];
        } else if (m_currGenInds[xa * m_popHeight + y]->fitness < ind_1->fitness) {

            if (ind_2 != NULL) {
                if (m_currGenInds[xa * m_popHeight + y]->fitness > ind_2->fitness) {
                    ind_2 = m_currGenInds[xa * m_popHeight + y];
                }
            } else {
                ind_2 = m_currGenInds[xa * m_popHeight + y];
            }
        }

        ya = y - 1;

        if (ya < 0) {
            ya += m_cfg->ypopsize;
        }

        if (m_currGenInds[x * m_popHeight + ya]->fitness > ind_1->fitness) {
            ind_2 = ind_1;
            ind_1 = m_currGenInds[x * m_popHeight + ya];
        } else if (m_currGenInds[x * m_popHeight + ya]->fitness < ind_1->fitness) {

            if (ind_2 != NULL) {
                if (m_currGenInds[x * m_popHeight + ya]->fitness > ind_2->fitness) {
                    ind_2 = m_currGenInds[x * m_popHeight + ya];
                }
            } else {
                ind_2 = m_currGenInds[x * m_popHeight + ya];
            }
        }

        ya = y + 1;

        if (ya >= m_cfg->ypopsize) {
            ya -= m_cfg->ypopsize;
        }

        if (m_currGenInds[x * m_popHeight + ya]->fitness > ind_1->fitness) {
            ind_2 = ind_1;
            ind_1 = m_currGenInds[x * m_popHeight + ya];
        } else if (m_currGenInds[x * m_popHeight + ya]->fitness < ind_1->fitness) {

            if (ind_2 != NULL) {
                if (m_currGenInds[x * m_popHeight + ya]->fitness > ind_2->fitness) {
                    ind_2 = m_currGenInds[x * m_popHeight + ya];
                }
            } else {
                ind_2 = m_currGenInds[x * m_popHeight + ya];
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

    std::shared_ptr<Individual> m_templateInd;
    Individual **m_currGenInds;
    Individual **m_nextGenInds;
    int m_inds_cnt;
    int m_popWidth, m_popHeight;

    int m_gensCount;

    std::shared_ptr<PopConfig> m_cfg;
    std::shared_ptr<IndData> m_indData;

    std::shared_ptr<Bitmap> image_;

    //Random rnd;

    std::vector<std::thread> m_threads;
    int m_number_of_threads_;

    std::queue<ThrTask> m_taskList;
    std::mutex task_list_mutex_;
};