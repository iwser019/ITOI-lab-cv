#ifndef IMAGEMATRIXFUNC_H
#define IMAGEMATRIXFUNC_H

#include "defs.h"
#include "imagedescriptor.h"
#include "imagematrix.h"
#include "imatrixedgeresolver.h"
#include "matrixedgeresolverfactory.h"
#include "util.h"

using namespace std;

/* работа с матрицами */

/*!
 * \brief Сложение матриц
 * \param matrix1 Матрица 1
 * \param matrix2 Матрица 2
 * \return Сумма матриц
 */
ImageMatrix matrixAdd(const ImageMatrix &matrix1, const ImageMatrix &matrix2);
/*!
 * \brief Сложение матрицы и числа
 * \param matrix Исходная матрица
 * \param value Число
 * \return
 */
ImageMatrix matrixAddScalar(ImageMatrix &matrix, double value);
/*!
 * \brief Вычитание матриц
 * \param matrix1 Уменьшаемая матрица
 * \param matrix2 Вычитаемая матрица
 * \return Разность матриц
 */
ImageMatrix matrixSubtract(ImageMatrix &matrix1, ImageMatrix &matrix2);
/*!
 * \brief Свёртка матрицы
 * \param matrix Исходная матрица
 * \param kernel Ядро свёртки
 * \param resolver Объект обработки края изображения
 * \return Обработанная матрица
 */
ImageMatrix matrixConvolute(const ImageMatrix &matrix,
							const ImageMatrix &kernel,
							IMatrixEdgeResolver *resolver = nullptr);
/*!
 * \brief Нормализация матрицы
 * \param matrix Исходная матрица
 * \return Нормализованная матрица
 */
ImageMatrix matrixNormalize(const ImageMatrix &matrix);

/* генерация ядер для свертки */

/*!
 * \brief Генерация ядра для частных производных
 * \param orientation Ориентация ядра (false - по X, true - по Y)
 * \param kernelType Тип ядра (0 - Собель, 1 - Щарр)
 * \return Ядро свёртки 3x3
 */
ImageMatrix kernelGeneratePartialDeriv(bool orientation = false,
									   int kernelType = KERNEL_SOBEL);
/*!
 * \brief Генерация ядра для сдвига на 1 px
 * \param hShift Сдвиг по горизонтали (0 - нет, 1 - влево, 2 - вправо)
 * \param vShift Сдвиг по вертикали (0 - нет, 1 - вверх, 2 - вниз)
 * \return Ядро свёртки 3x3
 */
ImageMatrix kernelGenerateShift1px(int hShift = 0, int vShift = 0);
/*!
 * \brief Генерация ядра-гауссиана
 * \param sigma Параметр среднеквадратичного отклонения
 * \return Ядро свёртки с радиусом 3*sigma
 */
ImageMatrix kernelGenerateGaussian(double sigma);
/*!
 * \brief Генерация ядра-гауссиана
 * \param radius Радиус ядра
 * \return Ядро свёртки с заданным радиусом
 */
ImageMatrix kernelGenerateGaussian(int radius);
/*!
 * \brief Генерация ядра-гауссиана (сепарабельное)
 * \param sigma Параметр среднеквадратичного отклонения
 * \param orientation Ориентация (false - по X, true - по Y)
 * \return Ядро свёртки с радиусом 3*sigma
 */
ImageMatrix kernelGenerateGaussianSeparate(double sigma,
										   bool orientation = false);
/*!
 * \brief Генерация ядра равномерного размытия
 * \param radius Радиус размытия
 * \return Ядро свёртки
 */
ImageMatrix kernelGenerateUniformBlur(int radius);
/*!
 * \brief Оператор Собеля
 * \param matrix Исходная матрица
 * \param kernelType Тип ядра (Собель/Щарр)
 * \param resolver Объект обработки края изображения
 * \param angleMap Вывод для карты углов
 * \return Обработанная матрица изображения
 */
ImageMatrix matrixFilterSobel(const ImageMatrix &matrix,
							  int kernelType = KERNEL_SOBEL,
							  IMatrixEdgeResolver *resolver = nullptr,
							  ImageMatrix *angleMap = nullptr);
/*!
 * \brief Вычисление следующего масштаба изображения
 * \param matrix Исходная матрица
 * \return Результирующая матрица (вдвое меньше)
 */
ImageMatrix matrixDownsample(const ImageMatrix &matrix);
/*!
 * \brief Вычисление максимального количества октав для изображения
 * \param matrix Исходная матрица
 * \return Максимальное число октав
 */
int matrixMaxAvailableOctaves(const ImageMatrix &matrix);
/*!
 * \brief Поиск "интересных" точек (оператор Моравека)
 * \param matrix Исходная матрица
 * \param threshold Пороговое значение
 * \param winRadius Радиус окна
 * \param resolver Обработчик края
 * \param responseMap Вывод для карты отклика
 * \return Набор точек
 */
QVector<Point> pointsMoravecPointOp(const ImageMatrix &matrix,
									const double &threshold,
									const int &winRadius,
									IMatrixEdgeResolver *resolver = nullptr,
									ImageMatrix *responseMap = nullptr);
/*!
 * \brief Поиск "интересных" точек (оператор Харриса)
 * \param matrix Исходная матрица
 * \param threshold Пороговое значение
 * \param winRadius Радиус окна
 * \param kernelType Тип ядра для вычисления произв. (0 - Собель, 1 - Щарр)
 * \param resolver Обработчик края
 * \param responseMap Вывод для карты отклика
 * \return Набор точек
 */
QVector<Point> pointsHarrisPointOp(const ImageMatrix &matrix,
								   const double &threshold,
								   const int &winRadius,
								   const int &kernelType = KERNEL_SOBEL,
								   IMatrixEdgeResolver *resolver = nullptr,
								   ImageMatrix *responseMap = nullptr);
/*!
 * \brief Фильтрация "интересных точек"
 * \param points Исходный набор точек
 * \param maxCount Максимальное количество точек
 * \return Отфильтрованные точки
 */
QVector<Point> pointsFilterANMS(QVector<Point> points, int maxCount);

/*!
 * \brief Построение дескрипторов инт. точек при пом. патчей
 * \param matrix Исходная матрица
 * \param points Набор интересных точек
 * \param blockSize Размер блока
 * \param gridSize Размер сетки в блоках
 * \param angleCount Количество корзин гистограммы
 * \param resolver Обработчик края
 * \return Набор дескрипторов интересных точек
 */
QVector<ImageDescriptor> descrBuildByPatch(const ImageMatrix &matrix,
										   const QVector<Point> points,
										   int blockSize = DESC_BLOCK_SIZE,
										   int gridSize = DESC_GRID_SIZE,
										   int angleCount = DESC_ANGLE_COUNT,
										   IMatrixEdgeResolver *resolver = nullptr);

#endif // IMAGEMATRIXFUNC_H
