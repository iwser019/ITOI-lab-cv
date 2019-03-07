#ifndef IMAGEMATRIXFUNC_H
#define IMAGEMATRIXFUNC_H

#include "defs.h"
#include "imagematrix.h"
#include "imatrixedgeresolver.h"

using namespace std;

/* работа с матрицами */

/*!
 * \brief Сложение матриц
 * \param matrix1 Матрица 1
 * \param matrix2 Матрица 2
 * \return Сумма матриц
 */
ImageMatrix matrixAdd(ImageMatrix &matrix1, ImageMatrix &matrix2);
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
ImageMatrix matrixConvolute(ImageMatrix &matrix,
							ImageMatrix &kernel,
							IMatrixEdgeResolver *resolver = nullptr);
/*!
 * \brief Нормализация матрицы
 * \param matrix Исходная матрица
 * \return Нормализованная матрица
 */
ImageMatrix matrixNormalize(ImageMatrix &matrix);

/* генерация ядер для свертки */

/*!
 * \brief Генерация ядра для частных производных
 * \param orientation Ориентация ядра (false - по X, true - по Y)
 * \param kernelType Тип ядра (0 - Собель, 1 - Щарр)
 * \param separate Сепарированное ядро
 * \return Ядро свёртки 3x3
 */
ImageMatrix kernelGeneratePartialDeriv(bool orientation = false,
									   int kernelType = 0);
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
 * \return Обработанная матрица изображения
 */
ImageMatrix matrixFilterSobel(ImageMatrix &matrix,
							  int kernelType = 0,
							  IMatrixEdgeResolver *resolver = nullptr);

#endif // IMAGEMATRIXFUNC_H
