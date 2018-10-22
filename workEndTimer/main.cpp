#include <iostream>
#include <chrono>
#include <thread>
#include <functional>
#include <mutex>

/*!
 * \brief startWithTimer выполняет лямбда-функцию func не более чем за timer миллисекунд
 * \param timer заданный предел работы для лямбды
 * \param func лямбда функция, которая должна выполняться
 * \param args аргументы для лямбда-функции
 * \return возвращает то, что возвращает функция
 */


template <typename T, typename ... Args>
std::shared_ptr<T> startWithTimer(long long int timer, std::function<T(Args...)> func, Args... args)
{
    const unsigned long long int deltaQuestion = 1; //как часто мы опрашиваем функцию на завершение

    std::shared_ptr<T> result = nullptr; //результат работы функции
    int funcCreated = 0; // 0 - не выполнена, 1 - заканчивает выполнятся, 2 - выполнена
    std::mutex m; //Мьютекс, чтобы не было конкуренции
    try
    {
        std::thread funcThread([&]()
        { //создаем тред для выполнения данной функции
            try
            {
            auto temp = func(args...);

            funcCreated = 1;
            m.lock();
            result.reset(new T(temp)); //Запускаем выполнение самой функции с данными параметрами и результат выполнения пишем в result
            funcCreated = 2; //Ставим то, что результат найден
            m.unlock();
            }
            catch(...)
            {
                std::cout << "error";
            }
        });

        std::thread timerThread([&]()
        { //создаем тред для отсчитывания таймера
            auto clock = std::chrono::system_clock::now(); //Замечаем начальное время
            while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - clock).count() < timer || funcCreated != 0) //Пока не вышло нужное количество времени
            {
                //m.lock(); //блокируем мьютекс
                if(funcCreated) //Если результат есть
                {
                    //m.unlock();
                    return; //ретерним тред
                }
                //m.unlock(); //разблокируем
                std::this_thread::sleep_for(std::chrono::milliseconds(deltaQuestion)); //Спим некоторое количество времени
            }
        });
        funcThread.detach(); //запускаем его и забываем
        timerThread.join(); //Ждем выполнения таймера. В случае, если funcThread завершится раньше он сделает resultExists = true, что в свою очередь завершит этот поток
    }
    catch(...)
    {
        std::cout << "error" << std::endl;
        return result;
    }
    return result; //Возвращаем результат
}

int main()
{
    for(int i = 0; i < 1000; i++)
    {
        auto start = std::chrono::system_clock::now(); //Для расчета сколько времени работал

        std::function<int(int)> f = [](int i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000)); //Что-нибудь делаем долгое, тут это эмулируется сном треда
            std::cout << "qq " << i << std::endl; //что-то выводим
            return i+1; //возвращаем какое-то значение <падение
        };
        std::shared_ptr<int> result = startWithTimer(1300, f, 123); //Получаем результат выполнения
        if(result) //если результатом являются данные
            std::cout  << "Result run: " << *result << std::endl; // вызываем наш метод и выводим его результат в консоль
        else //если nullptr
        {
            std::cout << "Function was late end time" << std::endl;
        }

        std::chrono::duration<double> diff = std::chrono::system_clock::now() - start; //засекаем конец выполнения
        std::cout << "TIME END PROGRAM: " << diff.count() << 's' << std::endl; //И выводим его
    }


    return 0;
}
