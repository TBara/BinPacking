#pragma once
#ifndef BIN_HPP
#define BIN_HPP

#include <vector>

class Bin
{
    public:
        Bin();
        ~Bin();
        int getCap();
        int getItems();
        void setCap(int cap);
        void setItems(int n);
        std::vector<int> getWeighsts();
        void steWeights(int weight);

        int firstFit(Bin bins);
        int bestFit(Bin bins);
        int firstFitDec(Bin bins);

    private:
        int capacity;
        int items;
        std::vector<int> weights;

};

#endif