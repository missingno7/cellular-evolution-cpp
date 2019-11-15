#pragma once
//
// Created by jiri on 14/11/19.
//

#include "cellular-evolution/varireg/varireg.h"
#include <string>
#include <iostream>

class PopConfig {
public:
PopConfig() {
        //reg = new VariReg();
    }

    /*
    // static method to create instance of Singleton class
static PopConfig getInstance()
    {
        if (single_instance == null)
            single_instance = new PopConfig();

        return single_instance;
    }
*/
    
    
    /*
    // For backwards compatibility
    public void initialize(std::string filename) {
        reg = new VariReg();
        LoadConfig(filename);
    }*/

    /*
public void LoadConfig(std::string filename) {
        std::string line = "";
        std::string cvsSplitBy = " ";

        cores = 16;
        std::cout<<"Detected threads: " << cores<<std::endl;

        try (BufferedReader br = new BufferedReader(new FileReader(filename))) {

            while ((line = br.readLine()) != null) {

                // use comma as separator
                std::string[] strVals = line.split(cvsSplitBy);

                if (strVals.length < 2) {
                    continue;
                }

                switch (strVals[0]) {
                    case "xpopsize":
                        xpopsize = Integer.parseInt(strVals[1]);
                        System.out.println("xpopsize = " + xpopsize);
                        break;
                    case "ypopsize":
                        ypopsize = Integer.parseInt(strVals[1]);
                        System.out.println("ypopsize = " + ypopsize);
                        break;

                    case "threads":
                        cores = Integer.parseInt(strVals[1]);
                        System.out.println("threads = " + cores);
                        break;

                    case "crossrate":
                        crossrate = Float.parseFloat(strVals[1]);
                        System.out.println("crossrate = " + crossrate);
                        break;

                    case "mutamount":
                        mutamount = Float.parseFloat(strVals[1]);
                        System.out.println("mutamount = " + mutamount);
                        break;
                    case "mutprob":
                        mutprob = Float.parseFloat(strVals[1]);
                        System.out.println("mutprob = " + mutprob);
                        break;
                    case "gennew":
                        gennew = Boolean.parseBoolean(strVals[1]);
                        System.out.println("gennew = " + gennew);
                        break;
                    case "drawpop":
                        drawpop = Boolean.parseBoolean(strVals[1]);
                        System.out.println("drawpop = " + drawpop);
                        break;
                    default:
                        Variable var = reg.items.get(strVals[0]);

                        if (var != null) {
                            if (strVals.length < 2) {
                                throw new java.lang.UnsupportedOperationException("Variabla " + strVals[0] + " has bad size " + strVals.length);
                            }
                            switch (var.tp) {
                                case INT:
                                    int[] iVals = new int[strVals.length - 1];
                                    for (int i = 1; i < strVals.length; i++) {
                                        iVals[i - 1] = Integer.parseInt(strVals[i]);

                                    }
                                    ((VInteger) var).setVal(iVals);
                                    break;
                                case LONG:
                                    long[] lVals = new long[strVals.length - 1];
                                    for (int i = 1; i < strVals.length; i++) {
                                        lVals[i - 1] = Long.parseLong(strVals[i]);
                                    }
                                    ((VLong) var).setVal(lVals);
                                    break;
                                case FLOAT:
                                    float[] fVals = new float[strVals.length - 1];
                                    for (int i = 1; i < strVals.length; i++) {
                                        fVals[i - 1] = Float.parseFloat(strVals[i]);

                                    }
                                    ((VFloat) var).setVal(fVals);
                                    break;
                                case DOUBLE:
                                    double[] dVals = new double[strVals.length - 1];
                                    for (int i = 1; i < strVals.length; i++) {
                                        dVals[i - 1] = Double.parseDouble(strVals[i]);
                                    }
                                    ((VDouble) var).setVal(dVals);
                                    break;
                                case BOOL:
                                    boolean[] bVals = new boolean[strVals.length - 1];
                                    for (int i = 1; i < strVals.length; i++) {
                                        bVals[i - 1] = Boolean.parseBoolean(strVals[i]);
                                    }
                                    ((VBool) var).setVal(bVals);
                                    break;
                            }
                        }

                        System.out.print(strVals[0] + " = ");
                        for (int i = 1; i < strVals.length; i++) {
                            if (i != strVals.length - 1) {
                                System.out.print(strVals[i] + ", ");
                            } else {
                                System.out.print(strVals[i]);
                            }
                        }
                        System.out.println("");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
*/

public:

VariReg reg;
int xpopsize=512;
int ypopsize=512;
int tsize=5;
float mutamount=0.1;
float mutprob=1.0;
float crossrate=0.5;
int cores=16;
bool gennew=true;
bool drawpop=true;

/*
    // static variable single_instance of type Singleton
private static PopConfig single_instance = null;
*/
};
