#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>

template<typename T>
struct Point {
    T x, y;
    Point(T x_val = 0, T y_val = 0) : x(x_val), y(y_val) {}
};

class Shape {
public:
    virtual double area() const = 0;
    virtual ~Shape() = default;
};

template<typename T>
class Polygon : public Shape {
protected:
    std::vector<Point<T>> vertices;
    static int instance_count;

    void check_validity() const {
        if (vertices.size() < 3) {
            throw std::invalid_argument("Фигура должна иметь минимум 3 точки");
        }
    }

public:
    Polygon(const std::vector<Point<T>>& vertex_list) : vertices(vertex_list) {
        check_validity();
        instance_count++;
    }

    Polygon(const Polygon& other) : vertices(other.vertices) {
        instance_count++;
    }

    Polygon& operator=(const Polygon& other) {
        if (this != &other) {
            vertices = other.vertices;
        }
        return *this;
    }

    virtual ~Polygon() {
        instance_count--;
    }

    static int get_instance_count() {
        return instance_count;
    }

    friend std::ostream& operator<<(std::ostream& output, const Polygon& polygon) {
        output << "Фигура с " << polygon.vertices.size() << " точками: ";
        for (const auto& vertex : polygon.vertices) {
            output << "(" << vertex.x << ", " << vertex.y << ") ";
        }
        return output;
    }
};

template<typename T>
int Polygon<T>::instance_count = 0;

template<typename T>
class Triangle : public Polygon<T> {
public:
    Triangle(const Point<T>& vertex1, const Point<T>& vertex2, const Point<T>& vertex3)
        : Polygon<T>({ vertex1, vertex2, vertex3 }) {}

    double area() const override {
        const auto& v = this->vertices;
        return std::abs((v[0].x * (v[1].y - v[2].y) +
            v[1].x * (v[2].y - v[0].y) +
            v[2].x * (v[0].y - v[1].y)) / 2.0);
    }

    friend std::ostream& operator<<(std::ostream& output, const Triangle& triangle) {
        output << "Треугольник: ";
        for (const auto& vertex : triangle.vertices) {
            output << "(" << vertex.x << ", " << vertex.y << ") ";
        }
        output << "Площадь: " << triangle.area();
        return output;
    }
};

template<typename T>
class Rectangle : public Polygon<T> {
public:
    Rectangle(const Point<T>& vertex1, const Point<T>& vertex2,
        const Point<T>& vertex3, const Point<T>& vertex4)
        : Polygon<T>({ vertex1, vertex2, vertex3, vertex4 }) {
        if (!check_rectangle_property()) {
            throw std::invalid_argument("Точки не образуют прямоугольник");
        }
    }

    bool check_rectangle_property() const {
        const auto& v = this->vertices;
        auto squared_distance = [](const Point<T>& a, const Point<T>& b) {
            return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
            };

        return squared_distance(v[0], v[1]) == squared_distance(v[2], v[3]) &&
            squared_distance(v[1], v[2]) == squared_distance(v[3], v[0]) &&
            squared_distance(v[0], v[2]) == squared_distance(v[1], v[3]);
    }

    double area() const override {
        const auto& v = this->vertices;
        auto width_value = std::sqrt((v[0].x - v[1].x) * (v[0].x - v[1].x) +
            (v[0].y - v[1].y) * (v[0].y - v[1].y));
        auto height_value = std::sqrt((v[1].x - v[2].x) * (v[1].x - v[2].x) +
            (v[1].y - v[2].y) * (v[1].y - v[2].y));
        return width_value * height_value;
    }

    friend std::ostream& operator<<(std::ostream& output, const Rectangle& rectangle) {
        output << "Прямоугольник: ";
        for (const auto& vertex : rectangle.vertices) {
            output << "(" << vertex.x << ", " << vertex.y << ") ";
        }
        output << "Площадь: " << rectangle.area();
        return output;
    }
};

namespace MatrixNamespace {



    class MatrixException : public std::exception {
    private:
        std::string error_message;
    public:
        MatrixException(const std::string& msg) : error_message(msg) {}
        const char* what() const noexcept override {
            return error_message.c_str();
        }
    };

    class Matrix {
    private:
        int row_count, column_count;
        int** matrix_data;
        static int matrix_count;

        void allocate_matrix_memory() {
            matrix_data = new int* [row_count];
            for (int i = 0; i < row_count; i++) {
                matrix_data[i] = new int[column_count]();
            }
        }

        void deallocate_matrix_memory() {
            if (matrix_data) {
                for (int i = 0; i < row_count; i++) {
                    delete[] matrix_data[i];
                }
                delete[] matrix_data;
            }
        }
    public:
        Matrix(int rows = 1, int columns = 1) : row_count(rows), column_count(columns), matrix_data(nullptr) {
            if (row_count <= 0 || column_count <= 0) {
                throw MatrixException("Неверные размеры матрицы");
            }
            allocate_matrix_memory();
            matrix_count++;
        }

        Matrix(const Matrix& other) : row_count(other.row_count), column_count(other.column_count), matrix_data(nullptr) {
            allocate_matrix_memory();
            for (int i = 0; i < row_count; i++) {
                for (int j = 0; j < column_count; j++) {
                    matrix_data[i][j] = other.matrix_data[i][j];
                }
            }
            matrix_count++;
        }

        Matrix& operator=(const Matrix& other) {
            if (this != &other) {
                deallocate_matrix_memory();
                row_count = other.row_count;
                column_count = other.column_count;
                allocate_matrix_memory();
                for (int i = 0; i < row_count; i++) {
                    for (int j = 0; j < column_count; j++) {
                        matrix_data[i][j] = other.matrix_data[i][j];
                    }
                }
            }
            return *this;
        }

        ~Matrix() {
            deallocate_matrix_memory();
            matrix_count--;
        }

        int& operator()(int row, int column) {
            if (row < 0 || row >= row_count || column < 0 || column >= column_count) {
                throw MatrixException("Выход за границы матрицы");
            }
            return matrix_data[row][column];
        }

        const int& operator()(int row, int column) const {
            if (row < 0 || row >= row_count || column < 0 || column >= column_count) {
                throw MatrixException("Выход за границы матрицы");
            }
            return matrix_data[row][column];
        }

        Matrix operator+(const Matrix& other) const {
            if (row_count != other.row_count || column_count != other.column_count) {
                throw MatrixException("Неверные размеры для сложения матриц");
            }
            Matrix result_matrix(row_count, column_count);
            for (int i = 0; i < row_count; i++) {
                for (int j = 0; j < column_count; j++) {
                    result_matrix.matrix_data[i][j] = this->matrix_data[i][j] + other.matrix_data[i][j];
                }
            }
            return result_matrix;
        }

        Matrix operator*(const Matrix& other) const {
            if (column_count != other.row_count) {
                throw MatrixException("Неверные размеры для умножения матриц");
            }
            Matrix result_matrix(row_count, other.column_count);
            for (int i = 0; i < row_count; i++) {
                for (int j = 0; j < other.column_count; j++) {
                    for (int k = 0; k < column_count; k++) {
                        result_matrix.matrix_data[i][j] += matrix_data[i][k] * other.matrix_data[k][j];
                    }
                }
            }
            return result_matrix;
        }


        static int get_matrix_count() {
            return matrix_count;
        }

        class Iterator {
        private:
            Matrix& matrix_ref;
            int current_row, current_column;
        public:
            Iterator(Matrix& m, int r = 0, int c = 0) : matrix_ref(m), current_row(r), current_column(c) {}

            int& operator*() {
                return matrix_ref(current_row, current_column);
            }

            Iterator& operator++() {
                current_column++;
                if (current_column >= matrix_ref.column_count) {
                    current_column = 0;
                    current_row++;
                }
                return *this;
            }

            bool operator!=(const Iterator& other) const {
                return current_row != other.current_row || current_column != other.current_column;
            }
        };

        Iterator begin() {
            return Iterator(*this, 0, 0);
        }

        Iterator end() {
            return Iterator(*this, row_count, 0);
        }

        friend std::ostream& operator<<(std::ostream& output, const Matrix& matrix) {
            for (int i = 0; i < matrix.row_count; i++) {
                for (int j = 0; j < matrix.column_count; j++) {
                    output << matrix.matrix_data[i][j] << " ";
                }
                output << std::endl;
            }
            return output;
        }
    };

    int Matrix::matrix_count = 0;

}

class Person {
protected:
    std::string person_name;
    int person_age;

    void validate_person_age() const {
        if (person_age < 0 || person_age > 150) {
            throw std::invalid_argument("Неверный возраст");
        }
    }

public:
    Person(const std::string& name = "", int age = 0) : person_name(name), person_age(age) {
        validate_person_age();
    }

    virtual ~Person() {
    }

    virtual void display_info() const {
        std::cout << "Имя: " << person_name << ", Возраст: " << person_age;
    }

    void set_person_name(const std::string& name) {
        person_name = name;
    }
    std::string get_person_name() const {
        return person_name;
    }

    int get_person_age() const {
        return person_age;
    }

    friend bool compare_by_age(const Person& person1, const Person& person2);
};

bool compare_by_age(const Person& person1, const Person& person2) {
    return person1.person_age < person2.person_age;
}

class Employee : virtual public Person {
protected:
    double employee_salary;

public:
    Employee(const std::string& name = "", int age = 0, double salary = 0.0)
        : Person(name, age), employee_salary(salary) {
    }

    virtual ~Employee() {
    }

    virtual void display_info() const override {
        Person::display_info();
        std::cout << ", Зарплата: " << employee_salary;
    }

    void set_employee_salary(double salary) {
        employee_salary = salary;
    }

    double get_employee_salary() const {
        return employee_salary;
    }
};

class Student : virtual public Person {
protected:
    double student_average_grade;

public:
    Student(const std::string& name = "", int age = 0, double average = 0.0)
        : Person(name, age), student_average_grade(average) {
    }

    virtual ~Student() {
    }

    virtual void display_info() const override {
        Person::display_info();
        std::cout << ", Средний балл: " << student_average_grade;
    }

    void set_student_average_grade(double average) {
        student_average_grade = average;
    }

    double get_student_average_grade() const {
        return student_average_grade;
    }
};

class WorkingStudent : public Employee, public Student {
public:
    WorkingStudent(const std::string& name = "", int age = 0, double salary = 0.0, double average = 0.0)
        : Person(name, age), Employee(name, age, salary), Student(name, age, average) {
    }

    ~WorkingStudent() {
    }


    void display_info() const override {
        Person::display_info();
        std::cout << ", Зарплата: " << employee_salary << ", Средний балл: " << student_average_grade;
    }
};

namespace SmartPointerNamespace {

    template<typename T>
    class SmartPointer {
    private:
        T* pointer;
        static int reference_count;

        class SmartPointerException : public std::exception {
        private:
            std::string error_message;
        public:
            SmartPointerException(const std::string& msg) : error_message(msg) {}
            const char* what() const noexcept override {
                return error_message.c_str();
            }
        };

    public:
        SmartPointer(T* p = nullptr) : pointer(p) {
            if (pointer) {
                reference_count++;
            }
        }

        SmartPointer(const SmartPointer& other) : pointer(other.pointer) {
            if (pointer) {
                reference_count++;
            }
        }

        SmartPointer& operator=(const SmartPointer& other) {
            if (this != &other) {
                if (pointer) {
                    reference_count--;
                    if (reference_count == 0) {
                        delete pointer;
                    }
                }
                pointer = other.pointer;
                if (pointer) {
                    reference_count++;
                }
            }
            return *this;
        }

        ~SmartPointer() {
            if (pointer) {
                reference_count--;
                if (reference_count == 0) {
                    delete pointer;
                }
            }
        }

        T& operator*() {
            if (!pointer) {
                throw SmartPointerException("Неверный указатель");
            }
            return *pointer;
        }

        T* operator->() {
            if (!pointer) {
                throw SmartPointerException("Неверный указатель");
            }
            return pointer;
        }

        static int get_reference_count() {
            return reference_count;
        }

        explicit operator bool() const {
            return pointer != nullptr;
        }
    };

    template<typename T>
    int SmartPointer<T>::reference_count = 0;

    template<typename T>
    class SmartPointer<T[]> {
    private:
        T* pointer;
        static int reference_count;

        class SmartPointerException : public std::exception {
        private:
            std::string error_message;
        public:
            SmartPointerException(const std::string& msg) : error_message(msg) {}
            const char* what() const noexcept override {
                return error_message.c_str();
            }
        };

    public:
        SmartPointer(T* p = nullptr) : pointer(p) {
            if (pointer) {
                reference_count++;
            }
        }
        SmartPointer(const SmartPointer& other) : pointer(other.pointer) {
            if (pointer) {
                reference_count++;
            }
        }

        SmartPointer& operator=(const SmartPointer& other) {
            if (this != &other) {
                if (pointer) {
                    reference_count--;
                    if (reference_count == 0) {
                        delete[] pointer;
                    }
                }
                pointer = other.pointer;
                if (pointer) {
                    reference_count++;
                }
            }
            return *this;
        }

        ~SmartPointer() {
            if (pointer) {
                reference_count--;
                if (reference_count == 0) {
                    delete[] pointer;
                }
            }
        }

        T& operator[](int index) {
            if (!pointer) {
                throw SmartPointerException("Неверный указатель");
            }
            return pointer[index];
        }


        T& operator*() {
            if (!pointer) {
                throw SmartPointerException("Неверный указатель");
            }
            return *pointer;
        }

        T* operator->() {
            if (!pointer) {
                throw SmartPointerException("Неверный указатель");
            }
            return pointer;
        }

        static int get_reference_count() {
            return reference_count;
        }
    };

    template<typename T>
    int SmartPointer<T[]>::reference_count = 0;

    class TestClass {
    public:
        void test_method() {
        }

        void show_message(const std::string& msg) {
            std::cout << "Сообщение: " << msg << std::endl;
        }
    };

}

int main() {
    setlocale(LC_ALL, "RUS");
    std::cout << "Задание 1" << std::endl;
    try {
        try {
            Point<int> point1(0, 0), point2(2, 0), point3(4, 4);
            Triangle<int> triangle(point1, point2, point3);
            std::cout << triangle << std::endl;

            Point<int> rect_point1(0, 0), rect_point2(1, 0), rect_point3(1, 1), rect_point4(0, 1);
            Rectangle<int> rectangle(rect_point1, rect_point2, rect_point3, rect_point4);
            std::cout << rectangle << std::endl;

            std::vector<Point<int>> invalid_points = { Point<int>(0,0), Point<int>(1,1) };
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Ошибка: " << e.what() << std::endl;
        }
    }
    catch (...) {
        std::cout << "Неизвестная ошибка" << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Задание 2" << std::endl;
    using namespace MatrixNamespace;

    try {
        Matrix matrix1(2, 2);
        matrix1(0, 0) = 1; matrix1(0, 1) = 1;
        matrix1(1, 0) = 1; matrix1(1, 1) = 1;

        Matrix matrix2(2, 2);
        matrix2(0, 0) = 2; matrix2(0, 1) = 2;
        matrix2(1, 0) = 2; matrix2(1, 1) = 2;

        Matrix sum_matrix = matrix1 + matrix2;
        Matrix product_matrix = matrix1 * matrix2;

        std::cout << "Матрица 1:\n" << matrix1;
        std::cout << "Матрица 2:\n" << matrix2;
        std::cout << "Сумма:\n" << sum_matrix;
        std::cout << "Произведение:\n" << product_matrix;

        std::cout << "Обход : ";
        for (auto iterator = matrix1.begin(); iterator != matrix1.end(); ++iterator) {
            std::cout << *iterator << " ";
        }
        std::cout << std::endl;

        std::cout << "Матриц : " << Matrix::get_matrix_count() << std::endl;
    }
    catch (const MatrixNamespace::MatrixException& e) {
        std::cout << "Ошибка матрицы: " << e.what() << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Задание 3" << std::endl;
    try {
        Person* persons[4];

        persons[0] = new Person("Петр", 19);
        persons[1] = new Employee("Евгения", 21, 60000);
        persons[2] = new Student("Ангелина", 18, 4.0);
        persons[3] = new WorkingStudent("Павел", 20, 32000, 3.3);

        for (int i = 0; i < 4; i++) {
            persons[i]->display_info();
            std::cout << std::endl;

            if (dynamic_cast<Employee*>(persons[i])) {
                std::cout << "Работник" << std::endl;
            }
            if (dynamic_cast<Student*>(persons[i])) {
                std::cout << "Студент" << std::endl;
            }
            if (dynamic_cast<WorkingStudent*>(persons[i])) {
                std::cout << "Студент-работник" << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << "Сравнение по возрасту: "
            << compare_by_age(*persons[0], *persons[1]) << std::endl;

        for (int i = 0; i < 4; i++) {
            delete persons[i];
        }

    }
    catch (const std::invalid_argument& e) {
    }
    std::cout <<"Задание 4" << std::endl;

    using namespace SmartPointerNamespace;


    try {
        try {
            SmartPointer<int> pointer1(new int(42));
            std::cout << "Значение: " << *pointer1 << std::endl;
            std::cout << "Ссылок: " << SmartPointer<int>::get_reference_count() << std::endl;

            SmartPointer<int> pointer2 = pointer1;
            std::cout << "Ссылок после копирования указателя: "
                << SmartPointer<int>::get_reference_count() << std::endl;

            SmartPointer<TestClass> object_pointer(new TestClass());
            object_pointer->test_method();
            object_pointer->show_message("Тест сообщения");

            SmartPointer<int[]> array_pointer(new int[5]);
            for (int i = 0; i < 5; i++) {
                array_pointer[i] = i ;
            }

            std::cout << "Массив: ";
            for (int i = 0; i < 5; i++) {
                std::cout << array_pointer[i] << " ";
            }
            std::cout << std::endl;

            SmartPointer<int> null_pointer;
        }
        catch (const std::exception& e) {
            std::cout << "Исключение: " << e.what() << std::endl;
        }
    }
    catch (...) {
        std::cout << "Неизвестное исключение" << std::endl;
    }

    return 0;
}
