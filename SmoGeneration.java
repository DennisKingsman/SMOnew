package com.example.interview.cmo.multitread;

import java.util.Arrays;
import java.util.Scanner;

public class SmoGeneration {

    private static Scanner scanner = new Scanner(System.in); // Класс для считывания с консоли

    public static void main(String[] args) {
        double workTime = scanner.nextDouble(); // временной отрезок работы СМО
        double lambda = scanner.nextDouble(); // параметр распределения Пуассона
        double mu = scanner.nextDouble(); //параметр экспоненциального распределения
        int m = scanner.nextInt(); // число потоков СМО

        double rho = lambda / mu;
        System.out.println("Показатель нагрузки СМО " + rho);
        double psi = rho/ m;
        System.out.println("Показатель нагрузки, приходящийся на один канал " + psi);
        double pNull = m / (rho + m );
        System.out.println("Вероятность простаивания всей системы " + pNull);
        System.out.println("Производительность каждого канала " + mu);
        System.out.println("Интенсивность простейшего входящего потока заявок " + lambda);
        System.out.println("Вероятности состояний");
        double sum = 0;
        double pOne = rho / (rho + m);
        System.out.println("Вероятность того, что все каналы заняты p1 = " + pOne);
        System.out.println("Вероятность того," +
                " что поступившая заявка будет принята к обслуживанию. p0 = " + pNull);
        double A =  lambda * m / (rho + m);
        System.out.println("Абсолютная пропускная способность A = " + A);
        System.out.println("Вероятность отказа " + pOne);
        double Q = m / (rho + m);
        System.out.println("Относительная пропуская способность " + Q);
        double matK = m * rho / (rho + m);
        System.out.println("Среднее число занятых каналов " + matK);
        double matNserved = rho / (rho + m);
        System.out.println("Среднее число заявок под обслуживанием " + matNserved);
        double matNsystem = rho / (rho + m);
        System.out.println("Среднее число заявок в системе " + matNsystem);
        double matTserved = rho / (lambda * m);
        System.out.println("Среднее время обслуживания одной заявки" +
                "(тех которые обслужили) " + matTserved);
        double matTall = 1 / (mu * (rho + m));
        System.out.println("Среднее время обслуживания одной заявки" +
                "(относящееся ко всем) " + matTall);
        double matTsystem = matTall;
        System.out.println("Среднее время пребывания заявки в системе " + matTsystem);

        System.out.println("Эмперические характеристики");
        int n = getPosition(lambda) ; //число поступивших заявок, которые мы хотим рассмотреть
        double[][] timeArray = getTimeArray(n, workTime, mu); //t
        int num;
        num = n > m ? n : m;
        double[][] out = new double[3][num]; //заявки на выходе: номер заявки, номер потока и время выхода из СМО
        double[][] in = initIn(m); //время
        double waitCounter = 0;
        int len = out[0].length - 1;
        for (int i = 0; i < n; i++) {
            Arrays.sort(in[0]);
            // if обслуживается else не обслуживается
            if (in[0][0] < timeArray[0][i]) {
                in[0][0] = timeArray[1][i] + timeArray[0][i];
                len--;
            } else { // ушла в отказ
                waitCounter++;
                in[0][0] += timeArray[1][i];
            }
            out[0][len] = i + 1;
            out[2][len] = in[0][0];
            out[1][len] = in[1][0];
        }
        double sys = 0;
        for (int i = 0; i < n; ++i) {
            sys += timeArray[1][i];
        }
        double sumN = 0;
        for (int i = 0; i < out[0].length; i++) {
            sumN += out[1][i];
        }
        double sumTimeN = 0;
        for (int i = 0; i < out[0].length; i++) {
            sumTimeN += out[2][i];
        }
        System.out.println("Среднее число занятых каналов " + sumN / (waitCounter / m));
        System.out.println("Среднее число заявок под обслуживанием " + (n - waitCounter) / n);
        System.out.println("Среднее число заявок в системе " + sumN / n);
        System.out.println("Среднее время обслуживания одной заявки" +
                "(тех которые обслужили) " + sumN / (sumTimeN + waitCounter));
        System.out.println("Среднее время обслуживания одной заявки" +
                "(относящееся ко всем) " + sumN / sumTimeN);
        System.out.println("Среднее время нахождения заявки в системе " + sys / (sumTimeN / n));
    }

    private static double[][] initIn(int m) {
        double[][] result = new double[2][m];
        for (int i = 0; i < m; i++) {
            result[1][i] = i + 1;
        }
        return result;
    }


    private static double getRandom(int min, int max) { //генератор псевдослучайных чисел в диапазоне
        max -= min;
        return (Math.random() * ++max) + min;
    }

    private static double getExp(double mu) { //с.в. с экспоненциальным з. распред.
        return (-(Math.log(1 - Math.random()) / mu));
    }

    //в массив заносим пары: момент поступления заявки и длительность обслуживания заявки
    //моделируя генератор времени поступления заявок
    private static double[][] getTimeArray(int n, double workTime, double mu) {
        double[][] result = new double[2][n];
        //n - весь поток (либо обсл либо нет)
        for (int i = 0; i < n; ++i) {
            result[0][i] = Math.random() * (workTime + 1); // от 0 до всей работы системы
            result[1][i] = getExp(mu);
        }
        return result;
    }

    private static int getPosition(double lambda) { //с.в. с пуассоновским з. распред.
        double l = Math.exp(-lambda);
        int k = 0;
        double p = 1;
        do {
            k++;
            // равновероятностное распределение чисел в заданном диапазоне [0, 1)
            double u = Math.random();
            p *= u;
        } while (p > l);
        return k - 1;
    }

}
