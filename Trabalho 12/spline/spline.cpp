//#include "stdafx.h"
#include "spline.h"
#include "math.h"

CSpline::CSpline( _point pt[], int np )
{
  InitData ( np );

	for( int i = 0; i < NP; i++ )
	{
		Px[i] = pt[i].dX;
		Py[i] = pt[i].dY;
	}
}

CSpline::CSpline( POINT_VECTOR pVec )
{
  InitData ( pVec.size() );

  for(int i=0; i < NP; i++)
  {
	  Px[i] = pVec.at(i).dX;
	  Py[i] = pVec.at(i).dY;
  }
}

// constructor
CSpline::CSpline(POINT_Q pt[], int np)
{
  InitData ( np );

	for( int i = 0; i < NP; i++ )
	{
		Px[i] = (float)pt[i].x;  
		Py[i] = (float)pt[i].y;
	}
}

CSpline::CSpline(float px[] , float py[] , int np) 
{
	InitData ( np );

	for( int i = 0; i < NP; i++ )
	{
		Px[i] = px[i];  
		Py[i] = py[i];
	}
}

void CSpline::InitData ( int np )
{
	NP = np;
	Px = new float[NP];
	Py = new float[NP];
	Ax = new float[NP];
	Ay = new float[NP];
	Bx = new float[NP];
	By = new float[NP];
	Cx = new float[NP];
	Cy = new float[NP];
	k = new float[NP];

	Mat[0] = new float[NP];
	Mat[1] = new float[NP];
	Mat[2] = new float[NP];
}

CSpline::~CSpline()
{
	delete[] Px;
	delete[] Py;
	delete[] Ax;
	delete[] Ay;
	delete[] Bx;
	delete[] By;
	delete[] Cx;
	delete[] Cy;
	delete[] k;
    delete[] Mat[0];
    delete[] Mat[1];
    delete[] Mat[2];
}

void CSpline::Generate() 
{
	float AMag , AMagOld;
  
  // vector A
	int i;
	for(i= 0 ; i<=NP-2 ; i++ ) 
	{
		Ax[i] = Px[i+1] - Px[i];
		Ay[i] = Py[i+1] - Py[i];
	}

	// k
	AMagOld = (float)sqrt(Ax[0]*Ax[0] + Ay[0]*Ay[0]);

	for(i=0 ; i<=NP-3 ; i++) 
	{
		AMag = (float)sqrt(Ax[i+1]*Ax[i+1] + Ay[i+1]*Ay[i+1]);
		k[i] = AMagOld / AMag;
		AMagOld = AMag;
	}

	k[NP-2] = 1.0f;

	// Matrix
	for(i=1; i<=NP-2;i++) 
	{
		Mat[0][i] = 1.0f;
		Mat[1][i] = 2.0f*k[i-1]*(1.0f + k[i-1]);
		Mat[2][i] = k[i-1]*k[i-1]*k[i];
	}

	Mat[1][0] = 2.0f;
	Mat[2][0] = k[0];
	Mat[0][NP-1] = 1.0f;
	Mat[1][NP-1] = 2.0f*k[NP-2];

	// 
	for(i=1; i<=NP-2;i++) 
	{
		Bx[i] = 3.0f*(Ax[i-1] + k[i-1]*k[i-1]*Ax[i]);
		By[i] = 3.0f*(Ay[i-1] + k[i-1]*k[i-1]*Ay[i]);
	}
	Bx[0] = 3.0f*Ax[0];
	By[0] = 3.0f*Ay[0];
	Bx[NP-1] = 3.0f*Ax[NP-2];
	By[NP-1] = 3.0f*Ay[NP-2];

	//
	MatrixSolve(Bx);
	MatrixSolve(By);

	for(i=0 ; i<=NP-2 ; i++ ) 
	{
		Cx[i] = k[i]*Bx[i+1];
		Cy[i] = k[i]*By[i+1];
	}
}

void CSpline::MatrixSolve(float B[]) 
{
	int i;
	float* Work = new float[NP];
	float* WorkB = new float[NP];
	
	for(i=0;i<=NP-1;i++) 
	{
		Work[i] = B[i] / Mat[1][i];
		WorkB[i] = Work[i];
	}

	int j;
	for(j=0; j<10 ; j++) 
	{ ///  need convergence judge
		Work[0] = (B[0] - Mat[2][0]*WorkB[1])/Mat[1][0];
		for(i=1; i<NP-1 ; i++ ) 
		{
			Work[i] = (B[i]-Mat[0][i]*WorkB[i-1]-Mat[2][i]*WorkB[i+1])
						/Mat[1][i];
		}
		Work[NP-1] = (B[NP-1] - Mat[0][NP-1]*WorkB[NP-2])/Mat[1][NP-1];

		for(i=0 ; i<=NP-1 ; i++ ) 
		{
			WorkB[i] = Work[i];
		}
	}
	for(i=0 ; i<=NP-1 ; i++ ) 
	{
		B[i] = Work[i];
	}
	delete[] Work;
	delete[] WorkB;
}

void CSpline::draw(QPainterEx* painter) 
{
	CCurve curve;
	for( int i = 0;i < NP - 1; i++ ) 
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		curve.draw    ( painter, Px[i], Py[i] );
	}
	
}

int CSpline::GetCurveCount()
{
	int nCount;
  nCount = 0;

  CCurve curve;
	for( int i = 0; i < NP; i++ )
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		nCount += curve.GetCount();
	}

	return nCount;
}

void CSpline::GetCurve( POINT_Q points[] )
{
  int PointCount;
  PointCount = 0;

  GetCurve( points, PointCount );
}

void CSpline::GetCurve(POINT_Q points[], int& PointCount)
{
	CCurve curve;
	for( int i = 0; i < NP - 1; i++ ) 
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i],  By[i], Cx[i], Cy[i] );
		curve.GetCurve( Px[i], Py[i], points, PointCount );
	}
	
}

void CSpline::GetCurve( _point pt[] )
{
  int nPointCount;
  nPointCount = 0;

	CCurve curve;

	for( int i = 0; i < NP ; i++ ) 
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		curve.GetCurve( Px[i], Py[i], pt, nPointCount );
	}
	
}

void CSpline::GenClosed() 
{
	float AMag , AMagOld , AMag0;
    
	// vector A
	int i;
	for(i = 0 ; i<=NP-2 ; i++ ) 
	{
		Ax[i] = Px[i+1] - Px[i];
		Ay[i] = Py[i+1] - Py[i];
	}
	Ax[NP-1] = Px[0] - Px[NP-1];
	Ay[NP-1] = Py[0] - Py[NP-1];

	// k
	AMag0 = AMagOld = (float)sqrt(Ax[0]*Ax[0] + Ay[0]*Ay[0]);

	for(i=0 ; i<=NP-2 ; i++) 
	{
		AMag = (float)sqrt(Ax[i+1]*Ax[i+1] + Ay[i+1]*Ay[i+1]);
		k[i] = AMagOld / AMag;
		AMagOld = AMag;
	}

	k[NP-1]=AMagOld/AMag0; 

	// Matrix
	for(i=1; i<=NP-1;i++) 
	{
		Mat[0][i] = 1.0f;
		//Mat[1][1] = 2.0f*k[i-1]*(1.0f + k[i-1]);
    Mat[1][i] = 2.0f*k[i-1]*(1.0f + k[i-1]);
		Mat[2][i] = k[i-1]*k[i-1]*k[i];
	}
	Mat[0][0] = 1.0f;
	Mat[1][0] = 2.0f*k[NP-1]*(1.0f + k[NP-1]);
	Mat[2][0] = k[NP-1]*k[NP-1]*k[0];

	// 
	for(i=1; i<=NP-1;i++) 
	{
		Bx[i] = 3.0f*(Ax[i-1] + k[i-1]*k[i-1]*Ax[i]);
		By[i] = 3.0f*(Ay[i-1] + k[i-1]*k[i-1]*Ay[i]);
	}
	Bx[0] = 3.0f*(Ax[NP-1] + k[NP-1]*k[NP-1]*Ax[0]);
	By[0] = 3.0f*(Ay[NP-1] + k[NP-1]*k[NP-1]*Ay[0]);

	//
	MatrixSolveEX(Bx);
	MatrixSolveEX(By);

	for(i=0 ; i<=NP-2 ; i++ ) 
	{
		Cx[i] = k[i]*Bx[i+1];
		Cy[i] = k[i]*By[i+1];
	}

	Cx[NP-1] = k[NP-1]*Bx[0];
	Cy[NP-1] = k[NP-1]*By[0];
}

void CSpline::MatrixSolveEX(float B[]) 
{
	float* Work  = new float[NP];
	float* WorkB = new float[NP];

	int i;
	int j;
	for(i=0;i<=NP-1;i++) 
	{
		Work[i] = B[i] / Mat[1][i];
		WorkB[i] = Work[i];
	}

	for(j=0 ; j<10 ; j++) 
	{  // need judge of convergence
		Work[0] = (B[0]-Mat[0][0]*WorkB[NP-1]-Mat[2][0]*WorkB[1])
				/Mat[1][0];
		for(i=1; i<NP-1 ; i++ ) 
		{
			Work[i] = (B[i]-Mat[0][i]*WorkB[i-1]-Mat[2][i]*WorkB[i+1])
						/Mat[1][i];
		}
		Work[NP-1] = (B[NP-1]-Mat[0][NP-1]*WorkB[NP-2]-Mat[2][NP-1]*WorkB[0])
						/Mat[1][NP-1];

		for(i=0 ; i<=NP-1 ; i++ ) 
		{
			WorkB[i] = Work[i];
		}
	}

	for(i=0 ; i<=NP-1 ; i++ ) 
	{
		B[i] = Work[i];
	}

  delete[] Work;
  delete[] WorkB;
}

void CSpline::drawClosed(QPainterEx* painter) 
{
	CCurve curve;
	for( int i = 0; i < NP; i++ )
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		curve.draw   ( painter ,  Px[i], Py[i] );
	}
}

QPolygon CSpline::GetClosedPolygon(QPainterEx* painter) 
{
	CCurve curve;

	for( int i = 0; i < NP; i++ )
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		curve.setPolygon( painter ,  Px[i], Py[i] );
	}

	return curve.createPolygon();
}

QPolygon CSpline::GetClosedPolygon(QPointF center) 
{
	CCurve curve;

	for( int i = 0; i < NP; i++ )
	{
		curve.PutCurve( Ax[i], Ay[i], Bx[i], By[i], Cx[i], Cy[i] );
		curve.setPolygon( center ,  Px[i], Py[i] );
	}

	return curve.createPolygon();
}

