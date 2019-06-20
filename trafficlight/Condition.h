#include "Stoplight.h"
#include "Environment.h"
#include "Stream.h"
#include <iostream>

using namespace std;

void updateOnArrive(Stream& stream) {
	stream.setKappa(stream.getKappa() + stream.getArrivedCarsForOneIteration());
	stream.setArrivedCarsStream(
			stream.getArrivedCarsStream()
					+ stream.getArrivedCarsForOneIteration());
}

void updateOnLeave(Stream& stream, double& avarageWaitingTime,
		const double& currentTime, double& sqrAvarageTime,
		double& avarageVariationTime, double& timeLastCar) {
	std::queue<double> helpQueue;
	helpQueue = stream.getArriveTime();
	timeLastCar = currentTime - helpQueue.front();
	avarageWaitingTime = avarageWaitingTime * stream.getServicedCarsStream()
			+ timeLastCar;
	sqrAvarageTime = sqrAvarageTime * stream.getServicedCarsStream()
			+ pow(timeLastCar, 2);
	helpQueue.pop();

	stream.setArriveTime(helpQueue);
	stream.setKappa(stream.getKappa() - 1);
	stream.setServicedCarsStream(stream.getServicedCarsStream() + 1);
	avarageWaitingTime /= stream.getServicedCarsStream();
	sqrAvarageTime /= stream.getServicedCarsStream();
	avarageVariationTime = sqrAvarageTime - pow(avarageWaitingTime, 2);
}

void carArrive(bool& true_flag, bool& true_flagOnce, bool& false_first,
		bool& false_second, bool& false_third, Environments& environmet,
		Stoplight& stoplight, Stream& stream, const double& currentTime) {
	// Последнее событие - уход заявки с потока
	false_first = false;
	false_second = false;
	false_third = false;
	true_flag = true;
	// Включаем флаг проверки определения текущего времени для потока
	true_flagOnce = true;
	environmet.setNextState(environmet.getCurrentState());
	stoplight.setNextState(stoplight.getCurrentState());
	//if ((stream.getKappa() == 1) || (stream.getServiceTime() == 0)) {
	stream.setServiceTime(currentTime);
	//}
	stream.generateService(environmet.getCurrentState());
}

bool timeless(const double& lastTime, const int& currEnviroment,
		const Stoplight& stoplight, const double& allTime) {
	if (currEnviroment == 1) {
		if (lastTime
				< (stoplight.getTimeGoodWeather().at(
						stoplight.getCurrentState() - 1) + allTime)) {
			return true;
		} else {
			return false;
		}
	} else {
		if (lastTime
				< (stoplight.getTimeBadWeather().at(
						stoplight.getCurrentState() - 1) + allTime)) {
			return true;
		} else {
			return false;
		}
	}
}

void updateData(const Stream& stream1, const Stream& stream2,
		double& avarageQueue1, double& avarageQueue2, int& counter,
		double& sqrAvarageQueue1, double& sqrAvarageQueue2,
		double& avarageVariationQueue1, double& avarageVariationQueue2) {
	avarageQueue1 = avarageQueue1 * counter + stream1.getKappa();
	avarageQueue2 = avarageQueue2 * counter + stream2.getKappa();
	sqrAvarageQueue1 = sqrAvarageQueue1 * counter + pow(stream1.getKappa(), 2);
	sqrAvarageQueue2 = sqrAvarageQueue2 * counter + pow(stream2.getKappa(), 2);
	counter++;
	avarageQueue1 /= counter;
	avarageQueue2 /= counter;
	sqrAvarageQueue1 /= counter;
	sqrAvarageQueue2 /= counter;
	avarageVariationQueue1 = sqrAvarageQueue1 - pow(avarageQueue1, 2);
	avarageVariationQueue2 = sqrAvarageQueue2 - pow(avarageQueue2, 2);
}

class Condition {
private:
	Environments environmet;
	Stoplight stoplight;
	Stream stream1;
	Stream stream2;
	int arrivedCars; // Общее число прибывших машин
	double allTime; // Общее время
	double currentTime; // Текущее время
	bool flag1; // Последнее событие: true - пришла заявка по первому потоку
	bool flag2; // Последнее событие: true - пришла заявка по второму потоку
	bool flag3; // Последнее событие: true - ушла заявка по первому потоку
	bool flag4; // Последнее событие: true - ушла заявка по второму потоку
	bool flagOnce1;
	bool flagOnce2;

	double avarageQueue1; // Средняя очередь по первому потоку
	double avarageQueue2; // Средняя очередь по второму потоку
	int counter; // Счетчик циклов светофора
	double avarageVariationQueue1; // Дисперия очереди по первому потоку
	double avarageVariationQueue2; // Дисперсия очереди по второму потоку
	double sqrAvarageQueue1; // Средняя очередь от квадрата по первому потоку
	double sqrAvarageQueue2; // Средняя очередь от квадрата по второму потоку
	double avarageWaitingTime1; // Среднее время ожидания по первому потоку
	double avarageWaitingTime2; // Среднее время ожидания по второму потоку
	double avarageVariationTime1; // Дисперсия времени ождиания по первому потоку
	double avarageVariationTime2; // Дисперсия времени ождиания по второму потоку
	double sqrAvarageTime1; // Среднее время от квадрата по первому потоку
	double sqrAvarageTime2; // Среднее время от квадрата по второму потоку
	double timeBadWeather; // Общее время прибывания системы в плохом состоянии
	double timeGoodWeather; // Общее время прибывания системы в хорошем состоянии
	double firstStreamTime; // Время, отведенное под обслуживание первого потока
	double secondStreamTime; // Время, отведенное под обслуживание второго потока
	double mathLoaderFirstStream; // Аналитическая загрузка первого потока
	double mathLoaderSecondStream; // Аналитическая загрузка второго потока

	double timeLastCar1; // Время ожидания последней заявки по первому потоку
	double timeLastCar2; // Время ожидания последней заявки по второму потоку
	bool isStationary; // Наличие стационарного режима в системе

public:
	Condition() {
		arrivedCars = 0;
		allTime = 0;
		currentTime = 0;
		flag1 = true;
		flag2 = true;
		flag3 = true;
		flag4 = true;
		flagOnce1 = false;
		flagOnce2 = false;
		avarageQueue1 = 0;
		avarageQueue2 = 0;
		counter = 0;
		avarageVariationQueue1 = 0;
		avarageVariationQueue2 = 0;
		sqrAvarageQueue1 = 0;
		sqrAvarageQueue2 = 0;
		avarageWaitingTime1 = 0;
		avarageWaitingTime2 = 0;
		avarageVariationTime1 = 0;
		avarageVariationTime2 = 0;
		sqrAvarageTime1 = 0;
		sqrAvarageTime2 = 0;
		timeBadWeather = 0;
		timeGoodWeather = 0;
		isStationary = true;
		timeLastCar1 = 0;
		timeLastCar2 = 0;
		firstStreamTime = 0;
		secondStreamTime = 0;
		mathLoaderFirstStream = 0;
		mathLoaderSecondStream = 0;
	}

	// Блок инициализации
	void initilization(const double& p11, const double& p22,
			const int& stoplightState, const int& enviromentState,
			const std::vector<double>& goodTime,
			const std::vector<double>& badTime, const double& lambda1Good,
			const double& lambda2Good, const double& lambda1Bad,
			const double& lambda2Bad, const int& kappa1, const int& kappa2,
			const double& fi1Good, const double& fi2Good, const double& fi1Bad,
			const double& fi2Bad, const double& p1, const double& q1,
			const double& p2, const double& q2)
			{
		environmet.setCurrentState(enviromentState);
		environmet.setProbability(p11, p22);
		environmet.setStationaryDistribution();

		stoplight.setCurrentState(stoplightState);
		stoplight.setTimeGoodWeather(goodTime);
		stoplight.setTimeBadWeather(badTime);

		stream1.setLambdaGoodWeather(lambda1Good);
		stream1.setLambdaBadWeather(lambda1Bad);
		stream1.setKappa(kappa1);
		if (kappa1 != 0) {
			stream1.setArrivedCarsStream(kappa1);
			std::queue<double> helpQueue1;
			for (int i = 0; i < kappa1; i++) {
				helpQueue1.push(0);
			}
			stream1.setArriveTime(helpQueue1);
		}

		stream1.setFiGoodWeather(fi1Good);
		stream1.setFiBadWeather(fi1Bad);
		stream1.setKsiMaxGoodWeather(static_cast<int>(goodTime[0] * fi1Good));
		stream1.setKsiMaxBadWeather(static_cast<int>(badTime[0] * fi1Bad));
		stream1.setP(p1);
		stream1.setQ(q1);
		stream1.setS(1 - q1 - p1);

		stream2.setLambdaGoodWeather(lambda2Good);
		stream2.setLambdaBadWeather(lambda2Bad);
		stream2.setKappa(kappa2);
		if (kappa2 != 0) {
			stream2.setArrivedCarsStream(kappa2);
			std::queue<double> helpQueue2;
			for (int i = 0; i < kappa2; i++) {
				helpQueue2.push(0);
			}
			stream2.setArriveTime(helpQueue2);
		}

		stream2.setFiGoodWeather(fi2Good);
		stream2.setFiBadWeather(fi2Bad);
		stream2.setKsiMaxGoodWeather(static_cast<int>(goodTime[2] * fi2Good));
		stream2.setKsiMaxBadWeather(static_cast<int>(badTime[2] * fi2Bad));
		stream2.setP(p2);
		stream2.setQ(q2);
		stream2.setS(1 - q2 - p2);
	}

	// Проверка наличия в системе стационарного режима [Методичка Е.В. Пройдаковой]
	void checkStationary() {
		isStationary = true;
		{
			double lhs = stream1.getLambdaGoodWeather()
					* stoplight.getCycleTimeGW();
			double rhs = stream1.getFiGoodWeather()
					* stoplight.getTimeGoodWeather().at(0);
			if (lhs > rhs) {
				isStationary = false;
			}
		}

		{
			double lhs = stream2.getLambdaGoodWeather()
					* stoplight.getCycleTimeGW();
			double rhs = stream2.getFiGoodWeather()
					* stoplight.getTimeGoodWeather().at(2);
			if (lhs > rhs) {
				isStationary = false;
			}
		}

		{
			double lhs = stream1.getLambdaBadWeather()
					* (stream1.getP() * 1 + stream1.getQ() * 2
							+ stream1.getS() * 3) * stoplight.getCycleTimeBW();
			double rhs = stream1.getFiBadWeather()
					* stoplight.getTimeBadWeather().at(0);
			if (lhs > rhs)
				isStationary = false;
		}

		{
			double lhs = stream2.getLambdaBadWeather()
					* (stream2.getP() * 1 + stream2.getQ() * 2
							+ stream2.getS() * 3) * stoplight.getCycleTimeBW();
			double rhs = stream2.getFiBadWeather()
					* stoplight.getTimeBadWeather().at(2);
			if (lhs > rhs)
				isStationary = false;
		}
		double sd1 = environmet.getStationaryDistribution1();
		double sd2 = environmet.getStationaryDistribution2();

		mathLoaderFirstStream = sd1 * stream1.getLambdaGoodWeather()
				* stoplight.getCycleTimeGW()
				+ sd2 * stream1.getLambdaBadWeather()
						* (stream1.getP() * 1 + stream1.getQ() * 2
								+ stream1.getS() * 3)
						* stoplight.getCycleTimeBW();
		mathLoaderFirstStream /= stream1.getFiGoodWeather()
				* stoplight.getTimeGoodWeather().at(0) * sd1
				+ stream1.getFiBadWeather()
						* stoplight.getTimeBadWeather().at(0) * sd2;

		mathLoaderSecondStream = sd1 * stream2.getLambdaGoodWeather()
				* stoplight.getCycleTimeGW()
				+ sd2 * stream2.getLambdaBadWeather()
						* (stream2.getP() * 1 + stream2.getQ() * 2
								+ stream2.getS() * 3)
						* stoplight.getCycleTimeBW();
		mathLoaderSecondStream /= stream2.getFiGoodWeather()
				* stoplight.getTimeGoodWeather().at(2) * sd1
				+ stream2.getFiBadWeather()
						* stoplight.getTimeBadWeather().at(2) * sd2;
	}

	// Определение следующего события
	void nextState(const double& randomVariable1, const double& randomVariable2,
			const double& randomVariable3, const double& randomVariable4,
			const double& randomVariable5) {

		// При неизвестном времени прихода следующей заявки по первому потоку - ищем его
		if (flag1) {
			stream1.generateNextTimeArrive(environmet.getCurrentState(),
					randomVariable1, randomVariable2);
		}

		// При неизвестном времени прихода следующей заявки по второму потоку - ищем его
		if (flag2) {
			stream2.generateNextTimeArrive(environmet.getCurrentState(),
					randomVariable4, randomVariable5);
		}

		// Определение текущего времени при окончании обслуживания заявки по 1 потоку
		if (flagOnce1) {
			// Окончание обслуживания становится текущем временем, если оно меньше времени прихода заявок по обоим потокам
			if ((stream1.getServiceTime() < stream2.getLastTime())
					&& ((stream1.getServiceTime() < stream1.getLastTime()))
					// И если оно успеевает попасть в промежуток обслужвания
					&& (stream1.getServiceTime()
							< allTime
									+ stoplight.getFirstStreamGreenTime(
											environmet.getCurrentState()))) {
				currentTime = stream1.getServiceTime();
				flagOnce1 = false;
			}
		}

		// Определение текущего времени при окончании обслуживания заявки по 2 потоку
		if (flagOnce2) {
			// Окончание обслуживания становится текущем временем, если оно меньше времени прихода заявок по обоим потокам
			if ((stream2.getServiceTime() < stream2.getLastTime())
					&& ((stream2.getServiceTime() < stream1.getLastTime()))
					// И если оно успеевает попасть в промежуток обслужвания
					&& (stream2.getServiceTime()
							< allTime
									+ stoplight.getSecondStreamGreenTime(
											environmet.getCurrentState()))) {
				currentTime = stream2.getServiceTime();
				flagOnce2 = false;
			}
		}

		// При зеленом светофоре для первого потока
		if (stoplight.getCurrentState() == 1) {
			// При не пустой очереди
			if (stream1.getKappa() != 0) {
				//  Время обсулживания последней заявки нестрого меньше текущего времени
				if ((stream1.getServiceTime() <= currentTime) &&
				// Число обслуженных машин строго меньше максимального количества
						(stream1.getKsiReal(environmet.getCurrentState())
								< stream1.getKsiMax(
										environmet.getCurrentState()))) {
					carArrive(flag3, flagOnce1, flag1, flag2, flag4, environmet,
							stoplight, stream1, currentTime);
					return;
				}
			}
		}

		// При зеленом светофоре для второго потока
		if (stoplight.getCurrentState() == 3) {
			// При не пустой очереди
			if (stream2.getKappa() != 0) {
				//  Время обсулживания последней заявки нестрого меньше текущего времени
				if ((stream2.getServiceTime() <= currentTime) &&
				// Число обслуженных машин строго меньше максимального количества
						(stream2.getKsiReal(environmet.getCurrentState())
								< stream2.getKsiMax(
										environmet.getCurrentState()))) {
					carArrive(flag4, flagOnce2, flag1, flag2, flag3, environmet,
							stoplight, stream2, currentTime);
					return;
				}
			}
		}

		if (stream1.getLastTime() < stream2.getLastTime()) {
			flag2 = false;
			flag3 = false;
			flag4 = false;
			// Если время прихода последней заявки по первому потоку, меньше чем время работы цикла светорфора
			if (timeless(stream1.getLastTime(), environmet.getCurrentState(),
					stoplight, allTime)) {
				// Следующее событие - пришла заявка по первому потоку
				flag1 = true;
				environmet.setNextState(environmet.getCurrentState());
				stoplight.setNextState(stoplight.getCurrentState());
				currentTime = stream1.getLastTime();
			}
			// В ином случае меняем состояние среды.
			else {
				// Если зеленый свет для первого потока, т.е. прошёл один цикл
				if (stoplight.getCurrentState() == 1) {
					updateData(stream1, stream2, avarageQueue1, avarageQueue2,
							counter, sqrAvarageQueue1, sqrAvarageQueue2,
							avarageVariationQueue1, avarageVariationQueue2);
				}
				flag1 = false;
				environmet.calculateNextState(randomVariable5);
				stoplight.calculateNextState();
			}
		} else {
			flag1 = false;
			flag3 = false;
			flag4 = false;
			if (timeless(stream2.getLastTime(), environmet.getCurrentState(),
					stoplight, allTime)) {
				// Следующее событие - пришла заявка по второму потоку
				flag2 = true;
				environmet.setNextState(environmet.getCurrentState());
				stoplight.setNextState(stoplight.getCurrentState());
				currentTime = stream2.getLastTime();
			} else {
				// Если зеленый свет для первого потока, т.е. прошёл один цикл
				if (stoplight.getCurrentState() == 3) {
					updateData(stream1, stream2, avarageQueue1, avarageQueue2,
							counter, sqrAvarageQueue1, sqrAvarageQueue2,
							avarageVariationQueue1, avarageVariationQueue2);
				}
				flag2 = false;
				environmet.calculateNextState(randomVariable5);
				stoplight.calculateNextState();
			}
		}
	}

	// Смена состояния
	void changeState() {
		if (flag1) {
			updateOnArrive(stream1);
		} else if (flag2) {
			updateOnArrive(stream2);
		} else if (flag3) {
			updateOnLeave(stream1, avarageWaitingTime1, currentTime,
					sqrAvarageTime1, avarageVariationTime1, timeLastCar1);
		} else if (flag4) {
			updateOnLeave(stream2, avarageWaitingTime2, currentTime,
					sqrAvarageTime2, avarageVariationTime2, timeLastCar2);
		}

		else {
			if (environmet.getCurrentState() == 1) {
				allTime += stoplight.getTimeGoodWeather().at(
						stoplight.getCurrentState() - 1);
				timeGoodWeather += stoplight.getTimeGoodWeather().at(
						stoplight.getCurrentState() - 1);
				if ((stoplight.getCurrentState() == 1) || (stoplight.getCurrentState() == 2)) {
					firstStreamTime += stoplight.getTimeGoodWeather().at(
							stoplight.getCurrentState() - 1);
				} else {
					secondStreamTime += stoplight.getTimeGoodWeather().at(
							stoplight.getCurrentState() - 1);
				}
			}

			if (environmet.getCurrentState() == 2) {
				allTime += stoplight.getTimeBadWeather().at(
						stoplight.getCurrentState() - 1);
				timeBadWeather += stoplight.getTimeBadWeather().at(
						stoplight.getCurrentState() - 1);
				if ((stoplight.getCurrentState() == 1) || (stoplight.getCurrentState() == 2)) {
					firstStreamTime += stoplight.getTimeBadWeather().at(
							stoplight.getCurrentState() - 1);
				} else {
					secondStreamTime += stoplight.getTimeBadWeather().at(
							stoplight.getCurrentState() - 1);
				}
			}

			currentTime = allTime;
			stream1.setServiceTime(currentTime);
			stream2.setServiceTime(currentTime);
		}

		arrivedCars = stream1.getArrivedCarsStream()
				+ stream2.getArrivedCarsStream();
		stoplight.changeState();

		environmet.changeState();
	}

	int getEnviromentState() const {
		return environmet.getCurrentState();
	}

	double getOneTimeCycle() const {
		if (stoplight.getCycleTimeGW() > stoplight.getCycleTimeBW()) {
			return stoplight.getCycleTimeGW();
		}
		return stoplight.getCycleTimeBW();
	}

	int getStoplightState() const {
		return stoplight.getCurrentState();
	}

	double getStationaryDistribution1() const {
		return environmet.getStationaryDistribution1();
	}

	double getStationaryDistribution2() const {
		return environmet.getStationaryDistribution2();
	}

	double getAllTime() const {
		return allTime;
	}

	double getCurrentTime() const {
		return currentTime;
	}

	int getKappa1() const {
		return stream1.getKappa();
	}

	int getKappa2() const {
		return stream2.getKappa();
	}

	double getArrivedCars() const {
		return arrivedCars;
	}

	double getServicedCars() const {
		return stream1.getServicedCarsStream() + stream2.getServicedCarsStream();
	}

	int getArrivedCarsStream1() const {
		return stream1.getArrivedCarsStream();
	}

	int getServicedCarsStream1() const {
		return stream1.getServicedCarsStream();
	}

	int getArrivedCarsStream2() const {
		return stream2.getArrivedCarsStream();
	}

	int getServicedCarsStream2() const {
		return stream2.getServicedCarsStream();
	}

	double getServiceTime1() const {
		return stream1.getServiceTime();
	}

	double getServiceTime2() const {
		return stream2.getServiceTime();
	}

	double getAvarageWaitingTime1() const {
		return avarageWaitingTime1;
	}

	double getAvarageWaitingTime2() const {
		return avarageWaitingTime2;
	}

	double getAvarageVariation1() const {
		return avarageVariationTime1;
	}

	double getAvarageVariation2() const {
		return avarageVariationTime2;
	}

	double getLoader1() const {
		if (allTime != 0) {
			return stream1.getLoader() / allTime;
		}
		return 0;
	}

	double getLoader2() const {
		if (allTime != 0) {
			return stream2.getLoader() / allTime;
		}
		return 0;
	}

	double getMathLoaderFirstStream() const {
		return mathLoaderFirstStream;
	}

	double getMathLoaderSecondStream() const {
		return mathLoaderSecondStream;
	}

	double getStatLoaderFirstStream() const {
		if (firstStreamTime != 0) {
			return stream1.getLoader() / firstStreamTime;
		}
		return 0;
	}

	double getStatLoaderSecondStream() const {
		if (secondStreamTime != 0) {
			return stream2.getLoader() / secondStreamTime;
		}
		return 0;
	}

	double getTimeGoodWeather() const {
		return timeGoodWeather;
	}

	double getTimeBadWeather() const {
		return timeBadWeather;
	}

	double getAvarageQueue1() const {
		return avarageQueue1;
	}

	double getAvarageQueue2() const {
		return avarageQueue2;
	}

	double getAvarageVariationQueue1() const {
		return avarageVariationQueue1;
	}

	double getAvarageVariationQueue2() const {
		return avarageVariationQueue2;
	}

	double getSystemLoader() const {
		if (allTime != 0) {
			return (stream1.getLoader() + stream2.getLoader()) / allTime;
		}
		return 0;
	}

	int getNumberOf1_1() const {
		return stream1.gerNumberOf1();
	}

	int getNumberOf2_1() const {
		return stream1.gerNumberOf2();
	}

	int getNumberOf3_1() const {
		return stream1.gerNumberOf3();
	}

	int getNumberOf1_2() const {
		return stream2.gerNumberOf1();
	}

	int getNumberOf2_2() const {
		return stream2.gerNumberOf2();
	}

	int getNumberOf3_2() const {
		return stream2.gerNumberOf3();
	}

	double getLamda1_1() const {
		return stream1.getLambdaGoodWeather();
	}

	double getLamda2_1() const {
		return stream2.getLambdaGoodWeather();
	}

	double getLamda1_2() const {
		return stream1.getLambdaBadWeather();
	}

	double getLamda2_2() const {
		return stream2.getLambdaBadWeather();
	}

	double getP1() const {
		return stream1.getP();
	}

	double getP2() const {
		return stream2.getP();
	}

	double getQ1() const {
		return stream1.getQ();
	}

	double getQ2() const {
		return stream2.getQ();
	}

	double getS1() const {
		return stream1.getS();
	}

	double getS2() const {
		return stream2.getS();
	}

	std::vector<double> getTimeGW() const {
		return stoplight.getTimeGoodWeather();
	}

	std::vector<double> getTimeBW() const {
		return stoplight.getTimeBadWeather();
	}

	bool getIsStationary() const {
		return isStationary;
	}

	double getTimeLastCar1() const {
		return timeLastCar1;
	}

	double getTimeLastCar2() const {
		return timeLastCar2;
	}
};
