
#include <QPainter>
#include <QTextStream>

#include <iostream>
#include <math.h>


#include "mainwindow.h"

#define DEFAULT_A -5
#define DEFAULT_B 5
#define DEFAULT_N 10
#define DEFAULT_PARAM 9
#define L2G(X,Y) (l2g ((X), (Y), min_y, max_y))
#define EPS 1e-15


static
    double f_0 (double x)
{
    Q_UNUSED(x);
    return 1;
}

static
    double f_1 (double x)
{
    return x;
}

static
    double f_2 (double x)
{
    return x * x;
}

static
    double f_3 (double x)
{
    return x * x * x;
}

static
    double f_4 (double x)
{
    return x * x * x * x;
}

static
    double f_5 (double x)
{
    return exp(x);
}

static
    double f_6 (double x)
{
    return 1 / (25 * pow (x, 2) + 1);
}


Window::Window (QWidget *parent)
    : QWidget (parent)
{
    a = DEFAULT_A;
    b = DEFAULT_B;
    n = DEFAULT_N;
    s = 1;
    func_id = 6;
    method_id = 3;
    p = 0;


    change_func ();
    func_method ();
}

QSize Window::minimumSizeHint () const
{
    return QSize (100, 100);
}

QSize Window::sizeHint () const
{
    return QSize (1000, 1000);
}


int Window::parse_command_line (int argc, char *argv[])
{
    if (argc == 1)
        return 0;

    if (argc == 2)
        return -1;

    if (argc == 3)
        return -2;

    if (sscanf (argv[1], "%lf", &a) != 1) 
    {
        std::cout << "неверный ввод a" << std::endl;
        return 1;
    }
    if (sscanf (argv[2], "%lf", &b) != 1) 
    {
        std::cout << "неверный ввод b" << std::endl;
        return 2;
    }
    if ((b - a < 1.e-6) || (a > b)) 
    {
        std::cout << "неверный ввод a or b" << std::endl;
        return 3;
    }
    if (sscanf (argv[3], "%d", &n) != 1) 
    {
        std::cout << "неверный ввод n" << std::endl;
        return 4;
    }
    if (n <= 0) {
        std::cout << "неверный ввод n<=0" << std::endl;
        return 5;
    }
    if (sscanf (argv[4], "%lf", &y) != 1) 
    {
        std::cout << "неверный ввод y" << std::endl;
        return 7;
    }
    if (sscanf (argv[5], "%d", &l) != 1) 
    {
        std::cout << "неверный ввод l" << std::endl;
        return 8;
    }
    if (((y < a) || (y > b))) {
        std::cout << "неверный ввод param" << std::endl;
        return 7;
    }
    if (argc > 6) {
        std::cout << "неверный ввод" << std::endl;
        return 8;
    }        


    return 0;
}
/*typedef struct {
    double a, b, c, d;
} SplineSegment;

// Решение трёхдиагональной системы (метод прогонки)
void solveTridiagonal(int n, double *diag, double *lower, double *upper, double *rhs, double *solution) {
    double *c_prime = (double*)malloc(n * sizeof(double));
    double *d_prime = (double*)malloc(n * sizeof(double));

    // Прямой ход прогонки
    c_prime[0] = upper[0] / diag[0];
    d_prime[0] = rhs[0] / diag[0];

    for (int i = 1; i < n; ++i) {
        double m = 1.0 / (diag[i] - lower[i-1] * c_prime[i-1]);
        c_prime[i] = (i < n-1) ? upper[i] * m : 0.0;
        d_prime[i] = (rhs[i] - lower[i-1] * d_prime[i-1]) * m;
    }

    // Обратный ход прогонки
    solution[n-1] = d_prime[n-1];
    for (int i = n-2; i >= 0; --i) {
        solution[i] = d_prime[i] - c_prime[i] * solution[i+1];
    }

    free(c_prime);
    free(d_prime);
}

// Построение кубического сплайна с заданными вторыми производными на границах
SplineSegment* buildCubicSpline(int n, double *x, double *y, double A, double B) {
    double *h = (double*)malloc((n) * sizeof(double));
    for (int i = 0; i < n; ++i) {
        h[i] = x[i+1] - x[i];
    }

    // Выделение памяти под матрицу и правую часть
    double *main_diag = (double*)malloc((n+1) * sizeof(double));
    double *lower_diag = (double*)malloc(n * sizeof(double));
    double *upper_diag = (double*)malloc(n * sizeof(double));
    double *rhs = (double*)malloc((n+1) * sizeof(double));

    // Граничные условия
    main_diag[0] = 2.0;
    upper_diag[0] = 1;
    rhs[0] = 3*(y[2] - y[1])/h[1] - A*h[1]/2;
    main_diag[n] = 2.0;
    lower_diag[n-1] = 1;
    rhs[n] = 3*(y[n]-y[n-1])/h[n-1] +B*h[n-1]/2;

    // Заполнение внутренних узлов
    for (int i = 1; i < n; ++i) {
        lower_diag[i] = h[i];
        main_diag[i] = 2.0 * (h[i-1] + h[i]);
        upper_diag[i] = h[i-1];
        rhs[i] = 3*(y[i] - y[i-1])*h[i] / h[i-1] +3*(y[i+1] - y[i])*h[i-1] / h[i];
    }
    lower_diag[n-1] = 1;


    // Решение системы для M_i
    double *M = (double*)malloc((n+1) * sizeof(double));
    solveTridiagonal(n+1, main_diag, lower_diag, upper_diag, rhs, M);

    // Вычисление коэффициентов сплайнов
    SplineSegment *splines = (SplineSegment*)malloc(n * sizeof(SplineSegment));
    for (int i = 0; i < n; ++i) {
        splines[i].a = y[i];
        splines[i].c = 3*(y[i+1] - y[i]) / (h[i]*h[i]) - (2.0 * M[i] + M[i+1]) / h[i];
        splines[i].b = M[i];
        splines[i].d = (M[i+1] + M[i] - 2*(y[i+1] - y[i])/ h[i] ) / h[i]*h[i];
    }

    // Освобождение памяти
    free(h);
    free(main_diag);
    free(lower_diag);
    free(upper_diag);
    free(rhs);
    free(M);

    return splines;
}*/

void Window::cubic(int n, const double *x, const double *f, double *spline_data) {
    double *h = (double*)malloc((n - 1) * sizeof(double));
    double *alpha = (double*)malloc((n - 1) * sizeof(double));
    double *l = (double*)malloc(n * sizeof(double));
    double *mu = (double*)malloc(n * sizeof(double));
    double *z = (double*)malloc(n * sizeof(double));
    double *m = (double*)malloc(n * sizeof(double));

    for (int i = 0; i < n - 1; ++i)
        h[i] = x[i + 1] - x[i];

    for (int i = 1; i < n - 1; ++i)
        alpha[i] = (3.0 / h[i]) * (f[i + 1] - f[i]) - (3.0 / h[i - 1]) * (f[i] - f[i - 1]);

    l[0] = 1.0;
    mu[0] = 0.0;
    z[0] = 0.0;

    for (int i = 1; i < n - 1; ++i) {
        l[i] = 2.0 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
        mu[i] = h[i] / l[i];
        z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
    }

    l[n - 1] = 1.0;
    z[n - 1] = 0.0;
    m[n - 1] = 0.0;

    for (int j = n - 2; j >= 0; --j) {
        m[j] = z[j] - mu[j] * m[j + 1];
    }

    for (int i = 0; i < n; ++i)
        spline_data[i] = m[i];

    free(h); free(alpha); free(l); free(mu); free(z);
}

double Window::evaluate_method2(double x_val, int n, const double *x, const double *f, const double *m) {
    int i;
    for (i = 0; i < n - 1; ++i) {
        if (x_val >= x[i] && x_val <= x[i + 1])
            break;
    }

    double h = x[i + 1] - x[i];
    double a = (x[i + 1] - x_val) / h;
    double b = (x_val - x[i]) / h;

    return a * f[i] + b * f[i + 1] +
    ((a * a * a - a) * m[i] + (b * b * b - b) * m[i + 1]) * (h * h) / 6.0;
}

/*double Window::evaluateSplineAt(const SplineSegment* spline, int n, double x_val, double* x_nodes) {
    // Находим нужный сегмент сплайна
    int i = 0;
    while (i < n && x_val > x_nodes[i+1]) {
        i++;
    }
    if (i >= n) i = n-1;  // Если x_val за правой границей
    
    double dx = x_val - x_nodes[i];
    const SplineSegment& s = spline[i];
    
    // Вычисляем значение кубического полинома: a + b*dx + c*dx² + d*dx³
    return s.a + s.b*dx + s.c*dx*dx + s.d*dx*dx*dx;
}*/


double Window::f_k_derivative(int k, double x, int l) {

    switch(l){
        case 0:{
            switch (k) {
        case 0: return 1.0;
        case 1: return x;
        case 2: return x * x;
        case 3: return x * x * x;
        case 4: return x * x * x * x;
        case 5: return exp(x);
        case 6: return 1.0 / (25.0 * x * x + 1.0);
        default: return 0.0;
    }
        }
        case 1:{
    switch (k) {
        case 0: return 0.0;
        case 1: return 1.0;
        case 2: return 2.0 * x;
        case 3: return 3.0 * x * x;
        case 4: return 4.0 * x * x * x;
        case 5: return exp(x);
        case 6: return -50.0 * x / ((25.0 * x * x + 1.0) * (25.0 * x * x + 1.0));
        default: return 0.0;
    }
    }
        case 2:{
        switch (k){
            case 0: return 0.0;
        case 1: return 0.0;
        case 2: return 2.0 ;
        case 3: return 6.0 * x;
        case 4: return 12.0 * x * x;
        case 5: return exp(x);
        case 6: return (3750 * x * x -50)/ ((25.0 * x * x + 1.0) * (25.0 * x * x + 1.0) * (25.0 * x * x + 1.0));
        default: return 0.0;
        }
        }
        default: return 0.0;
    }
}

int Window::factorial(int k) {
    if (k <= 1) return 1;
    return k * factorial(k - 1);
}

// Функция для вычисления разделенных разностей
void Window::dividedDifferences(int n, double * x, double * f, double * dd, int t) {
    //printf("изначальный массив в разделенных разностях: ");
    for (int u = 0; u < n; u++) {
        dd[u] = f[u];
        /*printf("%d",u);
        printf(": ");
        printf("%lf",dd[u]);*/
    }
    /*printf("\n");
    printf("%d",n);
    printf("\n");*/
    if (t == 0){
        for (int j = 1; j < n; j++) {
        for (int i = n - 1; i >= j; i--) {
            dd[i] = (dd[i] - dd[i - 1]) / (x[i] - x[i -j]);
        }
        }
    }
    else{
    for (int j = 1; j < n - t; j++) {
        for (int i = n - 1; i >= j; i--) {
            dd[i] = (dd[i] - dd[i - 1]) / (x[i+t-1] - x[i + t-1-j - t + 1]);
        }
        if (j == t){
            for (int r = 1; r < n; r++){
                f[r] = dd[r];
            }
        }
    }
    }
}

// Функция для вычисления значения многочлена Лагранжа в точке x
double Window::lagrangePolynomial(int n, double * x, double * d, double point) {
    double result = d[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        result = result * (point - x[i]) + d[i];
    }
    return result;
}

double Window::lagrangePolynomial_der(int n, double * x, double * d, double point, int t) {
    double result = d[n - t - 1];
    if (t >= 1){
        result = d[n-t-2];
        for (int i = n-t-2; i>=1; i--){
            result = result * (point - x[i-1]) + d[i-1];
        }
    }
    else{
    for (int i = n - t - 2; i >= 1; i--) {
        result = result * (point - x[i-1]) + d[i];
    }
    }
    return result;
}

double Window::derivative(int n, double * x, double * func, double y, int l){
    
    double value = 0;
    double * dd;
    double *arr;
    int w = 0;

    dd = (double *)malloc(n*sizeof(double));
    arr = (double *)malloc((n+1)*sizeof(double));

    for (int i = 0; i < n; i++){
        arr[i+1] = func[i];
    }
    dividedDifferences(n, x, func, dd, w);

    value = lagrangePolynomial(n, x, dd, y);
    for (int t = 1; t <= l; t++){
        dd[0] = (dd[0]-value)/(x[0]-y);

    for (int i = 0; i < n-t-2; i++){
        dd[i+1] = (dd[i+1] - dd[i])/(x[i+1]-y);
    }

    value = factorial(t)*lagrangePolynomial_der(n, x, dd, y, t);

    }


    free(dd);
    free(arr);


    return value;
}

double Window::derivative_discrepancy(int k, int n, double* x_vals, double y, int l, double * func){
    double res = fabs(derivative(n, x_vals, func, y, l) - f_k_derivative(k, y, l));
	return (res);
}

const char* Window::get_function_description(int k){
	switch(k){
		default: return "f(x) = 1";// k == 0
		case 1: return "f(x) = x";
		case 2: return "f(x) = x^2";
		case 3: return "f(x) = x^3";
		case 4: return "f(x) = x^4";
		case 5: return "f(x) = e^x";
		case 6: return "f(x) = 1/(25x^2 + 1)";
	}
	return "";
}

double Window:: splines_discrepancy(double x_0, int n, double* x_vals, double *f_vals, double*splines){
	double res = fabs(f(x_0) - evaluate_method2(x_0, n, x_vals, f_vals, splines));
	return (res);
}


int Window::x_val(int n, double a, double b, double* vector_x)
{
    double ptr = ((b - a)/(n - 1));
    for (int i = 0; i < n; i++) {
        vector_x[i] = (a + i * ptr);
    }
    return 0;
}

int Window::f_val(int n, int p, double norm, double* vector_x, double* vector_f_start) {
    for (int i = 0; i < n; i++) {
        vector_f_start[i] = f(vector_x[i]);
        if (i == ((n-1)/2)) {
            vector_f_start[i] = vector_f_start[i] + p * 0.1 * norm;
        }
    }
    return 0;
}

void Window::change_func ()
{
    func_id = (func_id + 1) % 7;

    switch (func_id)
    {
    case 0:
        f_name = "k = 0 f (x) = 1";
        f = f_0;
        break;
    case 1:
        f_name = "k = 1 f (x) = x";
        f = f_1;
        break;
    case 2:
        f_name = "k = 2 f (x) = x * x";
        f = f_2;
        break;
    case 3:
        f_name = "k = 3 f (x) = x * x * x";
        f = f_3;
        break;
    case 4:
        f_name = "k = 4 f (x) = x * x * x * x";
        f = f_4;
        break;
    case 5:
        f_name = "k = 5 f (x) = exp(x)";
        f = f_5;
        break;
    case 6:
        f_name = "k = 6 f (x) = 1 / (25 * pow (x, 2) + 1)";
        f = f_6;
        break;
    }

    update ();
}

void Window::func_method() {
    method_id = (method_id + 1) % 4;
    update ();
}

void Window::zoom_in()
{
    a = a/2;
    b = b/2;
    y = y/2;
    s-=1;
    update();
}

void Window::zoom_out()
{
    a = a * 2;
    b = b * 2;
    y = y * 2;
    s+=1;
    update();
}

void Window::double_n ()
{
    if (n < 100) 
    {
        n = 2 * n;
    }
    update();
}

void Window::a_half_n ()
{
    if  (n > 10)
    {
        if (n%2 == 1){
            n = (n-1)/2;
        }
        if (n%2 == 0) {
            n = n/2;
        }
    }
    update();
}

void Window::discrepancy_plus()
{
    p +=1;
    update();
}

void Window::discrepancy_minus()
{
    p-=1;
    update();
}

QPointF Window::l2g (double x_loc, double y_loc, double y_min, double y_max) //правильно переворачивает оси и учитывает масштаб
{
    double x_gl = (x_loc - a) / (b - a) * width ();
    double y_gl = (y_max - y_loc) / (y_max - y_min) * height ();
    return QPointF (x_gl, y_gl);
}


/// непосредственно рисуем график
void Window::paintEvent (QPaintEvent * /* event */)
{
    QPainter painter (this);
    double x1, x2, y1, y2;
    double max_y, min_y, norm;
    double delta_y, delta_x = (b-a)/width();
    double tpr = 0;
    double max_eps = 0;
    double A;
    double B;
    QPen pen_black(Qt::black, 0, Qt::SolidLine);
    QPen pen_red(Qt::red, 0, Qt::SolidLine);
    QPen pen_green(Qt::green, 0, Qt::SolidLine);
    QPen pen_blue(Qt::blue, 0, Qt::SolidLine);
    //число должно быть нечетным для метода с погрешностью
    if (n%2 == 0) {
        n += 1;
    }
    double* vector_x;
    double* vector_f_start;

    vector_x = (double*) malloc(sizeof(double) * n);
    vector_f_start = (double*) malloc(sizeof(double) * n);
    splines = new double[n];    
    A = f_k_derivative(func_id, a, 2);
    B = f_k_derivative(func_id, b, 2);


    max_y = min_y = 0;
    for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
    {
        y1 = f (x1);
        if (y1 < min_y)
            min_y = y1;
        if (y1 > max_y)
            max_y = y1;
    }

    delta_y = 0.001 * (max_y - min_y);

    norm = fabs(max_y);
    if (norm < fabs(min_y)) {
        norm = -min_y;
    }

    x_val(n, a, b, vector_x);
    f_val(n, p, norm, vector_x, vector_f_start);

    cubic(n, vector_x, vector_f_start, splines);
    
    char str6[700];
    char str7[700];
    std::snprintf(str6, 700, "Derivative of order %d in point x = %lf: %3f", l, y, derivative(n, vector_x, vector_f_start, y, l));
    std::snprintf(str7, 700, "Derivative discrepancy of order %d in point x = %lf: %3f", l, y, derivative_discrepancy(func_id, n, vector_x, y, l, vector_f_start));

    if (method_id == 0)
    {
        //оригинальная функция
        painter.setPen (pen_black);
        x1 = a;
        y1 = f(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            y2 = f(x2);
            // local coords are converted to draw coords
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x2);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));
        //метод кубических сплайнов
        painter.setPen (pen_green);
        x1 = a;
        //y1 = evaluateSplineAt(spline, n, x1, vector_x);
        y1 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
        max_eps = fabs(y1 - f(x1));
        std::cout << "max_eps = " << max_eps<< std::endl;
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            //y2 = evaluateSplineAt(spline, n, x1, vector_x);
            //y1 = buildCubicSpline(n, vector_x, vector_f_start, A, B);
            y2 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
            // local coords are converted to draw coords
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
            tpr = fabs(y1 - f(x1));
            if (fabs(max_eps) < fabs(tpr)) {
                max_eps = tpr;
            }
        }
        x2 = b;
        //y2 = evaluateSplineAt(spline, n, x1, vector_x);
        //y1 = buildCubicSpline(n, vector_x, vector_f_start, A, B);
        y2 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));
    }

    if (method_id == 1) 
    {
        //оригинальная функция
        painter.setPen (pen_black);
        x1 = a;
        y1 = f(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            y2 = f(x2);
            // local coords are converted to draw coords
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x2);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));


        painter.setPen ("blue");
		painter.drawText (0, 180, str6);



    }

    if (method_id == 2) 
    {
        //рисуем изначальную функцию
        painter.setPen (pen_black);
        x1 = a;
        y1 = f(x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            y2 = f(x2);
            // local coords are converted to draw coords
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = f(x2);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));


        //кубические сплайны
        painter.setPen (pen_green);
        x1 = a;
        y1 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
        //y1 = CALCULATING_THE_VALUE_OF_THE_INTERPOLATING_FUNCTION (n, param, vector_x, ff, vector_f_start, x1);
        max_eps = fabs(y1 - f(x1));
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            y2 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
            //y2 = CALCULATING_THE_VALUE_OF_THE_INTERPOLATING_FUNCTION (n, param, vector_x, ff, vector_f_start, x2);
            // local coords are converted to draw coords
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
            tpr = fabs(y1 - f(x1));
            if (fabs(max_eps) < fabs(tpr)) {
                max_eps = tpr;
            }
        }
        x2 = b;
        y2 = evaluate_method2(x1, n, vector_x, vector_f_start, splines);
        //y2 = CALCULATING_THE_VALUE_OF_THE_INTERPOLATING_FUNCTION (n, param, vector_x, ff, vector_f_start, x2);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));

        painter.setPen ("blue");
		painter.drawText (0, 180, str6);

    }

    if (method_id == 3) 
    { 
        painter.setPen (pen_green);
        max_y = min_y = 0;
        for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
        {
            y1 = splines_discrepancy(x1, n, vector_x, vector_f_start, splines);
            //y1 = ERROR_ESTIMATES_1(n, param, vector_x, ff, vector_f_start, x1);
            if (y1 < min_y)
                min_y = y1;
            if (y1 > max_y)
                max_y = y1;
        }
        if (max_y < 10e-20)
        {
            max_y = 10e-20;
            min_y = -10e-20;
        }

        norm = fabs(max_y);
        if (norm < fabs(min_y)) {
            norm = -min_y;
        }

        delta_y = 0.01 * (max_y - min_y);
        min_y -= delta_y;
        max_y += delta_y;


        //погрешность 
        painter.setPen (pen_green);
        x1 = a;
        y1 = splines_discrepancy(x1, n, vector_x, vector_f_start, splines);
        //y1 = ERROR_ESTIMATES_1(n, param, vector_x, ff, vector_f_start, x1);
        for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
        {
            y2 = splines_discrepancy(x1, n, vector_x, vector_f_start, splines);
            //y2 = ERROR_ESTIMATES_1(n, param, vector_x, ff, vector_f_start, x2);
            painter.drawLine (L2G(x1, y1), L2G(x2, y2));

            x1 = x2, y1 = y2;
        }
        x2 = b;
        y2 = splines_discrepancy(x1, n, vector_x, vector_f_start, splines);
        //y2 = ERROR_ESTIMATES_1(n, param, vector_x, ff, vector_f_start, x2);
        painter.drawLine (L2G(x1, y1), L2G(x2, y2));

        painter.setPen ("blue");
		painter.drawText (0, 180, str7);
    }

    // draw axis
    painter.setPen (pen_red);
    painter.drawLine (L2G(a, 0), L2G(b, 0));
    painter.drawLine (L2G(0, min_y), L2G(0, max_y));

    painter.setPen ("blue");


    //painter.drawText(0, 40, output);

    //вывод построчно
    int yPos = 40;
    QStringList lines = output.split('\n');
    for (const QString &line : lines) {
        painter.drawText(0, yPos, line);
        yPos += 15; // Межстрочный интервал (пиксели)
    }

    std::cout << "данные перерисованного графика" <<std::endl;
    std::cout <<f_name <<std::endl;
    std::cout <<"n = " << n<< std::endl;
    std::cout << "max_y = " <<  max_y <<std::endl;
    std::cout << "min_y = " <<  min_y <<std::endl;
    std::cout << "s = " << s << std::endl;
    std::cout << "methd_id = " << method_id << std::endl;
    std::cout << "p = " << p << std::endl;
    std::cout << "norm = " << norm << std::endl;


    free(vector_x);
    free(vector_f_start);


}
