﻿
// vtkMFCDlgExDlg.cpp: 구현 파일
//
#pragma region INCLUDE
#include "pch.h"
#include "framework.h"
#include "vtkMFCDlgEx.h"
#include "vtkMFCDlgExDlg.h"
#include "afxdialogex.h"

#include <vtkProperty.h>
#include <vtkArrowSource.h>

#include<vtkCleanPolyData.h>

#include<vtkPolyDataNormals.h>
#include<vtkSTLReader.h>

#include<vtkDecimatePro.h>
#include<vtkQuadricClustering.h>

#include<vtkWindowedSincPolyDataFilter.h>
#include<vtkPolyDataConnectivityFilter.h>

#include<vtkPlane.h>
#include<vtkClipPolyData.h>

#include<vtkTransform.h>
#include<vtkTransformPolyDataFilter.h>

#include<vtkLandmarkTransform.h>
#include<vtkIterativeClosestPointTransform.h>

#include<vtkDICOMImageReader.h>
#include<vtkImageViewer.h>

#include<VtkMarchingCubes.h>
#pragma endregion



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef vtkRenderingCore_AUTOINIT
#pragma message("vtkRenderingCore_AUTOINIT is defined")
#else
#pragma message("vtkRenderingCore_AUTOINIT is NOT defined")
#endif
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CvtkMFCDlgExDlg 대화 상자



CvtkMFCDlgExDlg::CvtkMFCDlgExDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VTKMFCDLGEX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CvtkMFCDlgExDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CvtkMFCDlgExDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONE, &CvtkMFCDlgExDlg::OnBnClickedButtonCone)
	ON_BN_CLICKED(IDC_BUTTON_EX_VTKPROPERTY, &CvtkMFCDlgExDlg::OnBnClickedButtonExVtkproperty)
END_MESSAGE_MAP()


// CvtkMFCDlgExDlg 메시지 처리기

BOOL CvtkMFCDlgExDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	if (this->GetDlgItem(IDC_STATIC_FRAME)) {
		this->InitVtkWindow(
			this->GetDlgItem(IDC_STATIC_FRAME)->GetSafeHwnd()
		);
		this->ResizeVtkWindow();
	}

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CvtkMFCDlgExDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	this->ResizeVtkWindow();
}

void CvtkMFCDlgExDlg::InitVtkWindow(void* hWnd) 
{
	//vtk window 생성
	if (m_vtkWindow == NULL)
	{
		//interactor 생성
		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();

		//Trackball camera 인터랙션 스타일 적용
		interactor->SetInteractorStyle(
			vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New()
		);

		//renderer 생성
		vtkSmartPointer<vtkRenderer> renderer =
			vtkSmartPointer<vtkRenderer>::New();

		//검은색 배경 적용
		renderer->SetBackground(0.0, 0.0, 0.0);

		//RenderWindow 생성 후, Dialog 핸들, Interactor, Renderer 설정
		m_vtkWindow = vtkSmartPointer<vtkRenderWindow>::New();
		m_vtkWindow->SetParentId(hWnd);
		m_vtkWindow->SetInteractor(interactor);
		m_vtkWindow->AddRenderer(renderer);
		m_vtkWindow->Render();
	}
}

void CvtkMFCDlgExDlg::ResizeVtkWindow()
{
	CRect rc;
	GetDlgItem(IDC_STATIC_FRAME)->GetClientRect(rc);
	m_vtkWindow->SetSize(rc.Width(), rc.Height());
}

void CvtkMFCDlgExDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CvtkMFCDlgExDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CvtkMFCDlgExDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CvtkMFCDlgExDlg::OnBnClickedOk2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CvtkMFCDlgExDlg::OnBnClickedButtonCone()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	vtkSmartPointer<vtkConeSource> coneSource =	vtkSmartPointer<vtkConeSource>::New();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	mapper->SetInputConnection(coneSource->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();

	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	renderer->AddActor(actor);
	renderer->SetBackground(.1, .2, .3);
	renderer->ResetCamera();

	//add camera, light
	vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
	vtkSmartPointer<vtkLight> newLight = vtkSmartPointer<vtkLight>::New();

	newLight->SetColor(1, 1, 0);
	newLight->SetFocalPoint(camera->GetFocalPoint());
	newLight->SetPosition(camera->GetPosition());
	renderer->AddLight(newLight);

	//camera->SetClippingRange(0.1, 10);
	//camera->SetFocalPoint(0, 0, 0);
	//camera->SetViewUp(0, 1, 0);
	//camera->SetPosition(0, 0, 5);



	m_vtkWindow->AddRenderer(renderer);
	m_vtkWindow->Render();


}

void CvtkMFCDlgExDlg::OnBnClickedButtonExVtkproperty()
{
	#pragma region DrawPolyData
	//vtkSmartPointer<vtkArrowSource> arrow = vtkSmartPointer<vtkArrowSource>::New();
	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	//// mapper에 polydata 연결
	//mapper->SetInputConnection(arrow->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//
	//actor->SetMapper(mapper);

	//actor->GetProperty()->SetColor(0, 1, 0); //색상 설정
	//actor->GetProperty()->SetOpacity(0.5); //불투명도 설정, 0.0~1.0 투명~불투명
	//actor->GetProperty()->SetPointSize(1.0); // vertex 크기
	//actor->GetProperty()->SetLineWidth(1.0); // line 굵기

	//actor->GetProperty()->SetRepresentation(VTK_POINTS); // VTK_POINTS, VTK_WIREFRAME, VTK_SURFACE
	//actor->GetProperty()->BackfaceCullingOn(); // 뒷면 제거
	//actor->GetProperty()->LightingOn(); // 조명 적용
	//actor->GetProperty()->ShadingOn(); // 그림자 효과

	//// 시각화
	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	////rendering
	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();
	#pragma endregion

	#pragma region CleanPolyData

	//vtkSmartPointer<vtkPoints> pPoints = vtkSmartPointer<vtkPoints>::New();
	//pPoints->InsertPoint(0, 0.0, 0.0, 0.0);
	//pPoints->InsertPoint(1, 0.0, 1.0, 0.0);
	//pPoints->InsertPoint(2, 1.0, 0.0, 0.0);
	//pPoints->InsertPoint(3, 1.0, 1.0, 0.0);
	//pPoints->InsertPoint(4, 0.0, 1.0, 0.0);
	//pPoints->InsertPoint(5, 1.0, 0.0, 0.0);

	//vtkSmartPointer<vtkCellArray> pPolys = vtkSmartPointer<vtkCellArray>::New();
	//pPolys->InsertNextCell(3);
	//pPolys->InsertCellPoint(0);
	//pPolys->InsertCellPoint(1);
	//pPolys->InsertCellPoint(2);

	//pPolys->InsertNextCell(3);
	//pPolys->InsertCellPoint(4);
	//pPolys->InsertCellPoint(3);
	//pPolys->InsertCellPoint(5);

	//vtkSmartPointer<vtkPolyData> pPolyData = vtkSmartPointer<vtkPolyData>::New();
	//pPolyData->SetPoints(pPoints);	//위치 정보
	//pPolyData->SetPolys(pPolys);		//형태 정보

	//int nPt = pPolyData->GetNumberOfPoints();	//점의 개수=6
	//int nPoly = pPolyData->GetNumberOfPolys();	//poly의 개수=2

	//vtkSmartPointer<vtkCleanPolyData> pClean = vtkSmartPointer<vtkCleanPolyData>::New();

	//pClean->SetInputData(pPolyData);
	//pClean->Update();

	//pPolyData->DeepCopy(pClean->GetOutput());
	//nPt = pPolyData->GetNumberOfPoints();	//점의 개수=4
	//nPoly = pPolyData->GetNumberOfPolys();	//poly의 개수=2

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputData(pPolyData);

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	#pragma endregion

	#pragma region Normal

	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example.stl");
	//stlReader->Update();

	//vtkSmartPointer<vtkPolyDataNormals> normals = vtkSmartPointer<vtkPolyDataNormals>::New();
	//normals->SetInputData(stlReader->GetOutput());
	//normals->ComputePointNormalsOn();	//Point Normal 계산
	//normals->ComputeCellNormalsOn();	//Cell Normal 계산
	//normals->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(normals->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	#pragma endregion

	#pragma region DECIMATION

	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example.stl");
	//stlReader->Update();


	////decimatePro
	//vtkSmartPointer<vtkDecimatePro> decimatepro = vtkSmartPointer<vtkDecimatePro>::New();
	//decimatepro->SetInputConnection(stlReader->GetOutputPort());
	//decimatepro->SetTargetReduction(0.9);	// 0.0~1.0, 0.9 = 90% 감소
	//decimatepro->PreserveTopologyOn();	//Topology 보존
	//decimatepro->Update();

	////quadricClustering
	//vtkSmartPointer<vtkQuadricClustering> qClustering = vtkSmartPointer<vtkQuadricClustering>::New();
	//qClustering->SetInputConnection(stlReader->GetOutputPort());
	//qClustering->SetNumberOfDivisions(10, 10, 10);	//x, y, z 축으로 나눌 개수
	//qClustering->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(qClustering->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	#pragma endregion
	
	#pragma region SMOOTHING
		
	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example.stl");
	//stlReader->Update();

	//vtkSmartPointer<vtkWindowedSincPolyDataFilter> smoothFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
	//smoothFilter->SetInputConnection(stlReader->GetOutputPort());
	//smoothFilter->SetNumberOfIterations(10000);	//반복 횟수
	//smoothFilter->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(smoothFilter->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();
	#pragma endregion

	#pragma region CONNECTIVITY CHECK
	
	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example_connectivity.stl");
	//stlReader->Update();

	//vtkSmartPointer<vtkPolyDataConnectivityFilter> conFilter = vtkSmartPointer<vtkPolyDataConnectivityFilter>::New();
	//conFilter->SetInputConnection(stlReader->GetOutputPort());

	//// 모든 영역 추출
	//conFilter->SetExtractionModeToAllRegions();

	//// 가장 큰 영역 추출
	////conFilter->SetExtractionModeToLargestRegion();	

	//// seed로 연결된 영역 추출
	////conFilter->AddSeed(id);	//seed 설정
	////conFilter->SetExtractionModeToCellSeededRegions();
	////conFilter->SetExtractionModeToPointSeededRegions();

	//// 특정 point와 가까운 점과 연결된 영역 추출
	////conFilter->SetClosestPoint(x, y, z);	//point 설정
	////conFilter->SetExtractionModeToClosestPointRegion();

	//conFilter->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(conFilter->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	//Sleep(10000);

	//conFilter->SetExtractionModeToLargestRegion();
	//conFilter->Update();

	//m_vtkWindow->Render();

	#pragma endregion

	#pragma region Clipping
	
	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example.stl");
	//stlReader->Update();

	//double center[3];
	//stlReader->GetOutput()->GetCenter(center); //mesh 중심점

	//vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	//plane->SetOrigin(center);
	//plane->SetNormal(1, 0, 0);	//x축으로 자름

	//vtkSmartPointer<vtkClipPolyData> clip = vtkSmartPointer<vtkClipPolyData>::New();
	//clip->SetInputConnection(stlReader->GetOutputPort());
	//clip->SetClipFunction(plane);
	//clip->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(clip->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	#pragma endregion

	#pragma region TRANSFORM
	
	//vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader->SetFileName("./data/example.stl");
	//stlReader->Update();

	//vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	//transform->Translate(10.0, 0, 0);	//이동
	//transform->RotateWXYZ(90, 0.0, 1.0, 0.0); //y축 중심으로 30도 회전

	//vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	//transformFilter->SetInputConnection(stlReader->GetOutputPort());
	//transformFilter->SetTransform(transform);
	//transformFilter->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(transformFilter->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	//actor->SetMapper(mapper);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();


	#pragma endregion

	#pragma region REGISTRATION_1
	
	//vtkSmartPointer<vtkPoints> point1 = vtkSmartPointer<vtkPoints>::New();
	//point1->InsertNextPoint(-1.5, 0, 0);
	//point1->InsertNextPoint(1.5, 0, 0);
	//point1->InsertNextPoint(0, 1, 0);

	//vtkSmartPointer<vtkCellArray> polys1 = vtkSmartPointer<vtkCellArray>::New();
	//polys1->InsertNextCell(3);
	//polys1->InsertCellPoint(0);
	//polys1->InsertCellPoint(1);
	//polys1->InsertCellPoint(2);

	//vtkSmartPointer<vtkPolyData> pd1 = vtkSmartPointer<vtkPolyData>::New();
	//pd1->SetPoints(point1);
	//pd1->SetPolys(polys1);

	//vtkSmartPointer<vtkPoints> point2 = vtkSmartPointer<vtkPoints>::New();
	//point2->InsertNextPoint(4, 2, 0);
	//point2->InsertNextPoint(2, 4, 0);
	//point2->InsertNextPoint(2, 2, 0);

	//vtkSmartPointer<vtkCellArray> polys2 = vtkSmartPointer<vtkCellArray>::New();
	//polys2->InsertNextCell(3);
	//polys2->InsertCellPoint(0);
	//polys2->InsertCellPoint(1);
	//polys2->InsertCellPoint(2);

	//vtkSmartPointer<vtkPolyData> pd2 = vtkSmartPointer<vtkPolyData>::New();
	//pd2->SetPoints(point2);
	//pd2->SetPolys(polys2);

	//vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper1->SetInputData(pd1);
	//mapper1->Update();

	//vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	//actor1->SetMapper(mapper1);
	//actor1->GetProperty()->SetRepresentationToWireframe();
	//actor1->GetProperty()->SetColor(1, 0, 0);

	//vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputData(pd2);
	//mapper2->Update();

	//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(mapper2);
	//actor2->GetProperty()->SetRepresentationToWireframe();
	//actor2->GetProperty()->SetColor(0, 1, 0);

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor1);
	//renderer->AddActor(actor2);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	//Sleep(1000);

	//vtkSmartPointer<vtkLandmarkTransform> landmarkTransform = vtkSmartPointer<vtkLandmarkTransform>::New();
	//landmarkTransform->SetSourceLandmarks(pd1->GetPoints());
	//landmarkTransform->SetTargetLandmarks(pd2->GetPoints());
	//landmarkTransform->SetModeToRigidBody();
	//landmarkTransform->Update();

	//actor1->SetUserTransform(landmarkTransform);
	//m_vtkWindow->Render();
	
	
	#pragma endregion

	#pragma region REGISTRATION_2

	//vtkSmartPointer<vtkSTLReader> stlReader1 = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader1->SetFileName("./data/example.stl");
	//stlReader1->Update();

	//vtkSmartPointer<vtkSTLReader> stlReader2 = vtkSmartPointer<vtkSTLReader>::New();
	//stlReader2->SetFileName("./data/example_smooth_transform.stl");
	//stlReader2->Update();

	//vtkSmartPointer<vtkPolyDataMapper> mapper1 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper1->SetInputConnection(stlReader1->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
	//actor1->SetMapper(mapper1);
	//actor1->GetProperty()->SetColor(1.0, 1.0, 0.5);
	//actor1->GetProperty()->SetOpacity(0.5); //불투명도 설정, 0.0~1.0 투명~불투명

	//vtkSmartPointer<vtkPolyDataMapper> mapper2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper2->SetInputConnection(stlReader2->GetOutputPort());

	//vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
	//actor2->SetMapper(mapper2);
	//actor2->GetProperty()->SetOpacity(0.5); //불투명도 설정, 0.0~1.0 투명~불투명

	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//renderer->AddActor(actor1);
	//renderer->AddActor(actor2);
	//renderer->SetBackground(0.1, 0.2, 0.3);
	//renderer->ResetCamera();

	//m_vtkWindow->AddRenderer(renderer);
	//m_vtkWindow->Render();

	//Sleep(1000);

	//vtkSmartPointer<vtkIterativeClosestPointTransform> icp = vtkSmartPointer<vtkIterativeClosestPointTransform>::New();
	//icp->SetSource(stlReader1->GetOutput());
	//icp->SetTarget(stlReader2->GetOutput());
	//icp->GetLandmarkTransform()->SetModeToRigidBody(); //Rigid Body로 설정
	//icp->SetMaximumNumberOfIterations(100); //최대 반복 횟수
	//icp->SetMaximumNumberOfLandmarks(50); //최대 랜드마크 개수
	//icp->Update();

	//actor1->SetUserTransform(icp);
	//m_vtkWindow->Render();


	#pragma endregion

	#pragma region DICOM IMAGE VIEWER
	
	//vtkSmartPointer<vtkDICOMImageReader> dcmReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	//dcmReader->SetFileName("./data/CT/CT.00002.00020.dcm");
	//dcmReader->Update();
	//
	//vtkSmartPointer<vtkImageViewer> imageViewer = vtkSmartPointer<vtkImageViewer>::New();
	//imageViewer->SetInputConnection(dcmReader->GetOutputPort());
	//imageViewer->SetRenderWindow(m_vtkWindow);
	//imageViewer->Render();
	
	#pragma endregion

	#pragma region MARCHING CUBES
	
	vtkSmartPointer<vtkDICOMImageReader> dcmReader = vtkSmartPointer<vtkDICOMImageReader>::New();
	dcmReader->SetDirectoryName("./data/CT");
	dcmReader->Update();

	vtkSmartPointer<vtkMarchingCubes> MCubes = vtkSmartPointer<vtkMarchingCubes>::New();
	MCubes->SetInputConnection(dcmReader->GetOutputPort());
	MCubes->SetValue(0, 100);	// 첫번째 iso-value 설정(id, value)
	MCubes->ComputeScalarsOff();	
	MCubes->ComputeScalarsOn();		// Normal 계산
	MCubes->Update();

	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(MCubes->GetOutputPort());
	
	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
	renderer->SetBackground(.1, .2, .3);
	renderer->ResetCamera();

	m_vtkWindow->AddRenderer(renderer);
	m_vtkWindow->Render();


	#pragma endregion

}
