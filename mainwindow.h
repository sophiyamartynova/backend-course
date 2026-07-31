#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QtWidgets>

class Window : public QWidget //наследуем публичные члены базового класса
{
    Q_OBJECT   //макрос для обработки сигналов

private:
    int func_id; //id функции
    int method_id; //id метода
    const char *f_name; //название функции в виде строки
    double a; //левый конец отрезка
    double b; //правый конец отрезка
    int n; //число отмеченных точек на отрезке
    int s; //переменная, отвечающая за приближение/отдаление
    //int param; //параметр для метода "движущимися многочленами"
    int p; //параметр, отвечающий за прибавление/вычитание невязки к центру функции на отрезке
    double (*f) (double);  //указатель на функцию
    double *splines;
    int l;
    double y;

public:
    Window (QWidget *parent);

    QSize minimumSizeHint () const;
    QSize sizeHint () const;

    int parse_command_line (int argc, char *argv[]); //функция считывания с терминала вводных значений


    QPointF l2g (double x_loc, double y_loc, double y_min, double y_max); //Вызывается, когда окно нуждается в перерисовке. В нашем случае при вызове update()

public slots: //Для создания сигналов в Qt
    void change_func (); //меняет функцию

    void func_method(); //меняет метод

    void zoom_out();  //отдаляет

    void zoom_in(); //приближает

    void double_n(); //удваивает число точек

    void a_half_n(); //уменьшает число точек

    void discrepancy_plus(); //метод для невязки

    void discrepancy_minus(); //метод для невязки


protected: //доступны в классе и производных от этого класса, в остальных недоступны
    //вводим точки приближения
    typedef struct {
    double a, b, c, d;
    } SplineSegment;
    void cubic(int n, const double *x, const double *f, double *spline_data);

    double splines_discrepancy(double x_0, int n, double* x_vals, double *f_vals, double*splines);
    double evaluate_method2(double x, int n, const double *x_points, const double *f, const double *spline_data);
    int x_val(int n, double a, double b, double* vector_x);
    //void solveTridiagonal(int n, double *diag, double *lower, double *upper, double *rhs, double *solution);

    double f_k_derivative(int k, double x, int l);
    //double evaluateSplineAt(const SplineSegment* splines, int n, double x_val, double* x_nodes);
    double derivative_discrepancy(int k, int n, double* x_vals, double y, int l, double * func);

    double f_error(int k, double x, int p, double max_abs_value);

    double derivative(int n, double * x, double * func, double y, int l);
    double lagrangeDerivative(int n, double *x, double *f, double y, int k);
    double evaluatePolynomial(int n, double *x, double *coeffs, double point);
    void dividedDifferences(int n, double *x, double *f, double *dd, int w);
    int factorial(int k);
    double lagrangePolynomial(int n, double * x, double * d, double point);
    double lagrangePolynomial_der(int n, double * x, double * d, double point, int t);
    const char* get_function_description(int k);
    //void solveTridiagonal(int n, double *diag, double *lower, double *upper, double *rhs, double *solution);
    

    //вычисляет необходимый размер массива для хранения разделенных разностей
    //int SIZE (int n, int param);
    //Ввод начального вектора F, с учетом параметра p (невязка в начальной точке)
    int f_val(int n, int p, double norm, double* vector_x, double* vector_f_start);
    //SplineSegment* buildCubicSpline(int n, double *x, double *y, double A, double B);

    //вычисляется массив разделенных разностей
    /*int ARRAY_F_RIGHT(int n, int param, double* vector_x, double* vector_f, double* vector_f_start);

    //функция определяет, какому промежутку из приближаемых значений принадлежит точка. возвращает номер отрезка
    int DEFINING_THE_NUBBER_OF_INTERVAL (int n, int param, double* vector_x, double x);

    //функция вычисляет разделенные разности
    double SEPARETED_DIFFERENCE (double f_1, double f_2, double x_1, double x_2);

    //считается значение интерполирующей функции в точке х
    double CALCULATING_THE_VALUE_OF_THE_INTERPOLATING_FUNCTION (int n, int param, double* vector_x, double* vector_f, double* vector_f_right, double x);
    
    
    //метод нахождения "двойных" разделенных разностей для метода 2
    int FIND_SECOND_SPLIT_DIFFERENCE(int n, double* vector_x, double* vector_f_start, double* ff_2);

    //находим значения d 2...n-1
    int FIND_D_2_N_1(int n, double* vector_x, double* d, double* ff_2);

    //находим значения d 1,n методом экстраполяции в приграничных узлах
    int FIND_D_1_N(int n, double* vector_x, double* d, double* vector_f_start, double* sep_dif_2);

    //подсчет массива с - коэффицента для полинома в методе 2
    int CALCULATE_C (int n, double* vector_x, double* vector_f_start, double* ff_2, double* d, double* c);

    //method 2
    double METHOD_2 (int n, double* vector_x, double* vector_f, double* d, double* ff_2, double* sep_dif_2, double* c, double x);

    //погрешность
    double ERROR_ESTIMATES_1 (int n, int param, double* vector_x, double* vector_f,  double* vector_f_right, double x);

    //погрешность для метода 2
    double ERROR_ESTIMATES_2 (int n, double* vector_x, double* vector_f_start, double* d, double* ff_2, double* sep_dif_2, double* c, double x);
    */

    void paintEvent (QPaintEvent *event);
};


#endif

