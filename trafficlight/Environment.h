#pragma once
#include <vector>

class Environments {
    std::vector<double> p1;
    std::vector<double> p2;
    int currentState;  // 1 - "хорошее" состояние внешней среды, 2 - "плохое" состояние внешней среды
    int nextState;  // Следующее состояние, 0 - в данный момент состояние не определено
    std::vector<double> stationaryDistribution; // Cтационарное распределение

public:
    Environments() {
        p1.emplace_back(0.95);
        p1.emplace_back(1 - 0.95);
        p2.emplace_back(0.25);
        p2.emplace_back(1 - 0.25);
        currentState = 1;
        nextState = 0;
    }

    // Задание вероятностей смены вшеней среды
    void setProbability(const double& p11, const double& p22) {
        p1.at(0) = p11;
        p1.at(1) = 1 - p11;
        p2.at(0) = 1 - p22;
        p2.at(1) = p22;
    }

    // Определение следующего событя
    void calculateNextState(const double& randomVariable) {
        // Текущее состояние - "хорошее"
        if (currentState == 1) {
            if ((0 <= randomVariable) && (p1.at(0) > randomVariable)) {
                nextState = 1;
            } else {
                nextState = 2;
            }
        }
        // Текущее состояние - "плохое"
        else {
            if ((0 <= randomVariable) && (p2.at(0) > randomVariable))
                nextState = 1;
            else {
                nextState = 2;
            }
        }
    }

    void changeState() {
        currentState = nextState;
        nextState = 0;
    }

    // Определение стационарного распределения
    void setStationaryDistribution() {
        // Решение уравнения стационарного распределения: (q*)P = q*
        if (p1.at(1) != p2.at(0)) {
            double q1 = -p2.at(0) / (-p1.at(1) - p2.at(0));
            double q2 = 1 - q1;
            stationaryDistribution.emplace_back(q1);
            stationaryDistribution.emplace_back(q2);
        } else {
            stationaryDistribution.emplace_back(p1.at(0));
            stationaryDistribution.emplace_back(p1.at(1));
        }
    }

    double getStationaryDistribution1() const {
        return stationaryDistribution.at(0);
    }

    double getStationaryDistribution2() const {
        return stationaryDistribution.at(1);
    }

    int getCurrentState() const {
        return currentState;
    }

    void setCurrentState(const int& state) {
        currentState = state;
    }

    int getNextState() const {
        return nextState;
    }

    void setNextState(const int& state) {
        nextState = state;
    }
};
