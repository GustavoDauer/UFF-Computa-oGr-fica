#ifndef _SPLINE_H
#define _SPLINE_H

#include <typedata.h>
#include "curve.h"
#include "calipergraph_global.h"

/**
 * Classe que trata das splines.
 */
class CALIPERGRAPHSHARED_EXPORT CSpline
{
  private:

    float* Px;
    float* Py;
    float* Ax;
    float* Ay;
    float* Bx;
    float* By;
    float* Cx;
    float* Cy;
    float*  k;
    float*  Mat[3];

    int  NP;

  public:
    /**
     * Construtor.
     */
    CSpline( _point pt[],  int np);

    /**
     * Construtor.
     */
    CSpline( POINT_VECTOR );

    /**
     * Construtor.
     */
    CSpline( POINT_Q pt[], int np);

    /**
     * Construtor.
     */
    CSpline( float px[] ,float py[], int np);

    /**
     * Destrutor.
     */
    ~CSpline();

    /**
     * @brief GetCurveCount - Conta as curvas
     * @return nCount Retorna a contagem das curvas.
     */
    int GetCurveCount();

    /**
     * @brief GetCurve - Obtém a curva.
     * @param points
     */
    void GetCurve( POINT_Q points[] );

    /**
     * @brief GetCurve - Obtém a curva.
     * @param points Vetor de pontos.
     * @param PointCount Contagem de pontos.
     */
    void GetCurve( POINT_Q points[], int& PointCount );

    /**
     * @brief GetCurve - Obtém a curva.
     * @param pt Vetor de pontos.
     */
    void GetCurve( _point pt[] );

    /**
     * @brief Generate - Gera a cubic spline.
     */
    void Generate();

    /**
     * @brief draw - Desenha a spline.
     */
    void draw(QPainterEx*);

    /**
     * @brief GenClosed - Fecha a cubic spline.
     */
    void GenClosed();

    /**
     * @brief drawClosed - Desenha o polígono fechado (Fecho Convexo ?).
     * @param QPainterEx.
     */
    void drawClosed(QPainterEx*);

    /**
     * @brief GetClosedPolygon - Obtém o polígono fechado (Fecho Convexo ?)
     * @param painter QPainterEx.
     * @return Retorna o polígono fechado (Fecho Convexo ?).
     */
    QPolygon GetClosedPolygon(QPainterEx* painter = 0) ;

    /**
     * @brief GetClosedPolygon - Obtém o polígono fechado (Fecho Convexo ?).
     * @param center Ponto central.
     * @return Retorna o polígono fechado (Fecho Convexo ?).
     */
    QPolygon GetClosedPolygon(QPointF center) ;

  private:
    /**
     * @brief InitData - Inicializa os vetores, matrizes e demais variáveis.
     * @param np
     */
    void InitData ( int np );

    /**
     * @brief MatrixSolve - Converge a Matriz.
     * @param B Matriz.
     */
    void MatrixSolve(float B[]);

    /**
     * @brief MatrixSolveEX - Matriz Tridiagonal + elementos de [0][0], [N-1][N-1]
     * @param B
     */
    void MatrixSolveEX(float B[]);
};

#endif
