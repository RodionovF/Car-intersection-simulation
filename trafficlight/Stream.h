#pragma once
#include <queue>
#include <math.h>

class Stream {
	double lambdaGoodWeather; // Параметр интенсивности входного потока при хорошем состоянии внешней среды
	double lambdaBadWeather; // Параметр интенсивности входного потока при плохом состоянии внешней среды
	double p;  // Вероятность прихода в пачке одной машины
	double q;  // Вероятность прихода в пачке двух машин
	double s;  // Вероятность прихода в пачке трех машин
	double fiGoodWeather; // Интенсивность пропускной способности перекрестка при хорошем состоянии внешней среды
	double fiBadWeather; // Интенсивность пропускной способности перекреста при плохом состоянии внешней среды
	int kappa;  // Число машин в очереди по потоку
	int ksiMaxGoodWeather; // Максимальная пропускная способность перекреста при хорошем состоянии внешней среды
	int ksiMaxBadWeather; //  Максимальная пропускная способность перекреста при плохом состоянии внешней среды
	int ksiRealGoodWeather; // Реальное число обслуженных машин за время работы перекрестка при хорошем состоянии внешней среды
	int ksiRealBadWeather; // Реальное число обслуженных машин за время работы перекрестка при плохом состоянии внешней среды
	int eta;  // Число машин, пришедших на перекресток

	std::queue<double> arriveTime;  // Время прихода каждой заявки
	double lastTime;  // Время пришедшей последней заявки
	int arrivedCarsStream;  // Пришло всего машин по потоку
	int servicedCarsStream;  // Обслужено всего машин по потоку

	int arrivedCarsForOneIteration;  // Число пришедших машин за цикл
	double serviceTime;  // Время обслуживания
	double loader;  // Загрузка системы

	int numberOf1;  // Количество раз, когда пришла 1 машина
	int numberOf2;  // Количество раз, когда пришли 2 машины
	int numberOf3;  // Количество раз, когда пришли 3 машины

public:
	Stream() {
		lambdaGoodWeather = 0.25;
		lambdaBadWeather = 0.25;
		p = 0.5;
		q = 0.3;
		s = 0.2;
		fiGoodWeather = 0.25;
		fiBadWeather = 0.25;
		kappa = 0;
		ksiMaxBadWeather = 7;
		ksiMaxGoodWeather = 7;
		ksiRealGoodWeather = 0;
		ksiRealBadWeather = 0;
		eta = 0;
		lastTime = 0;
		arrivedCarsStream = 0;
		servicedCarsStream = 0;
		arrivedCarsForOneIteration = 0;
		serviceTime = 0;
		loader = 0;
		numberOf1 = 0;
		numberOf2 = 0;
		numberOf3 = 0;
	}

	void generateService(const int& enviroment) {
		if (enviroment == 1) {
			serviceTime += pow(fiGoodWeather, -1);
			loader += pow(fiGoodWeather, -1);
		} else if (enviroment == 2) {
			serviceTime += pow(fiBadWeather, -1);
			loader += pow(fiBadWeather, -1);
		}
	}

	void generateNextTimeArrive(const int& enviroment,
			const double& randomValue1, const double& randomValue2) {
		double timeNextArrive = 0;
		arrivedCarsForOneIteration = 0;
		if (enviroment == 1) {
			timeNextArrive = -log(randomValue1) / lambdaGoodWeather; // Время прихода i-ой машины
			lastTime += timeNextArrive;
			arriveTime.push(lastTime);  // Сохраняем время прихода i-ой машины
			++arrivedCarsForOneIteration;  // Увеличиваем число пришедших машин
		} else if (enviroment == 2) {
			timeNextArrive = -log(randomValue1) / lambdaBadWeather; // Время прихода i-ой пачки машин машины
			double n = 0;
			if ((0 <= randomValue2) && (p > randomValue2)) {
				n = 1;
				numberOf1++;
			} else if ((p <= randomValue2) && ((p + q) > randomValue2)) {
				n = 2;
				numberOf2++;
			} else if (((p + q) <= randomValue2)
					&& ((p + q + s) >= randomValue2)) {
				n = 3;
				numberOf3++;
			}
			lastTime += timeNextArrive;
			for (int i = 0; i < n; ++i) {
				arriveTime.push(lastTime); // Сохраняем время прихода машин в i-ой пачке
				++arrivedCarsForOneIteration; // Запоминаем число пришедших машин
			}
		}
	}

	void setLambdaGoodWeather(const double& lamdaGood) {
		lambdaGoodWeather = lamdaGood;
	}

	double getLambdaGoodWeather() const {
		return lambdaGoodWeather;
	}

	void setLambdaBadWeather(const double& lamdaBad) {
		lambdaBadWeather = lamdaBad;
	}

	double getLambdaBadWeather() const {
		return lambdaBadWeather;
	}

	void setP(const double& _p) {
		p = _p;
	}

	double getP() const {
		return p;
	}

	void setQ(const double& _q) {
		q = _q;
	}

	double getQ() const {
		return q;
	}

	void setS(const double& _s) {
		s = _s;
	}

	double getS() const {
		return s;
	}

	void setFiGoodWeather(const double& fiGood) {
		fiGoodWeather = fiGood;
	}

	double getFiGoodWeather() const {
		return fiGoodWeather;
	}

	void setFiBadWeather(const double& fiBad) {
		fiBadWeather = fiBad;
	}

	double getFiBadWeather() const {
		return fiBadWeather;
	}

	void setKappa(const int& _kappa) {
		kappa = _kappa;
	}

	int getKappa() const {
		return kappa;
	}

	void setKsiMaxGoodWeather(const int& ksiMaxGood) {
		ksiMaxGoodWeather = ksiMaxGood;
	}

	int getKsiMaxGoodWeather() const {
		return ksiMaxGoodWeather;
	}

	void setKsiMaxBadWeather(const int& ksiMaxBad) {
		ksiMaxBadWeather = ksiMaxBad;
	}

	int getKsiMaxBadWeather() const {
		return ksiMaxBadWeather;
	}

	void setKsiRealGoodWeather(const int& ksiRealGood) {
		ksiRealGoodWeather = ksiRealGood;
	}

	int getKsiRealGoodWeather() const {
		return ksiRealGoodWeather;
	}

	void setKsiRealBadWeather(const int& ksiRealBad) {
		ksiRealBadWeather = ksiRealBad;
	}

	int getKsiRealBadWeather() const {
		return ksiRealBadWeather;
	}

	void setEta(const int& _eta) {
		eta = _eta;
	}

	int getEta() const {
		return eta;
	}

	void setArriveTime(const std::queue<double>& arrive) {
		arriveTime = arrive;
	}

	std::queue<double> getArriveTime() const {
		return arriveTime;
	}

	void setLastTime(const double& last) {
		lastTime = last;
	}

	double getLastTime() const {
		return lastTime;
	}

	void setArrivedCarsStream(const int& arrivedCars) {
		arrivedCarsStream = arrivedCars;
	}

	int getArrivedCarsStream() const {
		return arrivedCarsStream;
	}

	void setServicedCarsStream(const int& servicedCars) {
		servicedCarsStream = servicedCars;
	}

	int getServicedCarsStream() const {
		return servicedCarsStream;
	}

	void setArrivedCarsForOneIteration(const int& count) {
		arrivedCarsForOneIteration = count;
	}

	int getArrivedCarsForOneIteration() const {
		return arrivedCarsForOneIteration;
	}

	double getServiceTime() const {
		return serviceTime;
	}

	void setServiceTime(const double& time) {
		serviceTime = time;
	}

	int getKsiReal(const int& enviroment) const {
		if (enviroment == 1) {
			return ksiRealGoodWeather;
		} else {
			return ksiRealBadWeather;
		}
	}

	int getKsiMax(const int& enviroment) const {
		if (enviroment == 1) {
			return ksiMaxGoodWeather;
		} else {
			return ksiMaxBadWeather;
		}
	}

	double getLoader() const {
		return loader;
	}

	int gerNumberOf1() const {
		return numberOf1;
	}

	int gerNumberOf2() const {
		return numberOf2;
	}

	int gerNumberOf3() const {
		return numberOf3;
	}
};
