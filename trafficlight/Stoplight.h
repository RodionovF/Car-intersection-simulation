#pragma once
#include <vector>

class Stoplight {
    int currentState;  // 1, 2, 3, 4 - состояния светофора
    int nextState;  // 0 - в данный момент состояние не определено
    std::vector<double> timeGoodWeather;  // Время работы светофора при хорошем состоянии внешней среды
    std::vector<double> timeBadWeather;  // Время работы светофора при плохом состоянии внешней среды

public:
    Stoplight() {
        currentState = 1;
        nextState = 0;
    }

    // Циклический алгоритм смены состояния обслуживающего устройства
    void calculateNextState() {
        if (currentState % 4 != 0) {
            nextState = currentState + 1;
        } else {
            nextState = 1;
        }
    }

    void changeState() {
        currentState = nextState;
        nextState = 0;
    }

    int getCurrentState() const {
        return currentState;
    }

    void setCurrentState(const int& state) {
        currentState = state;
    }

    int getNextState() const {
        return currentState;
    }

    void setNextState(const int& state) {
        nextState = state;
    }

    void setTimeGoodWeather(const std::vector<double>& time) {
        timeGoodWeather = time;
    }

    std::vector<double> getTimeGoodWeather() const {
        return timeGoodWeather;
    }

    void setTimeBadWeather(const std::vector<double>& time) {
        timeBadWeather = time;
    }

    std::vector<double> getTimeBadWeather() const {
        return timeBadWeather;
    }

    double getCycleTimeGW() const {
        return timeGoodWeather.at(0) + timeGoodWeather.at(1) + timeGoodWeather.at(2) + timeGoodWeather.at(3);
    }

    double getCycleTimeBW() const {
        return timeBadWeather.at(0) + timeBadWeather.at(1) + timeBadWeather.at(2) + timeBadWeather.at(3);
    }

    double getFirstStreamGreenTime(const int&  enviroment) const {
        if (enviroment == 1) {
            return timeGoodWeather.at(0);
        } else {
            return timeBadWeather.at(0);
        }
    }

    double getSecondStreamGreenTime(const int& enviroment) const {
        if (enviroment == 1) {
            return timeGoodWeather.at(2);
        } else {
            return timeBadWeather.at(2);
        }
    }
};
