# Our House Graphics Project

서강대학교 CSE4170/AIE4012 기초 컴퓨터 그래픽스
HW3 & HW5 통합 구현 프로젝트

---

## 📦 프로젝트 소개

OpenGL (Core Profile)을 활용하여 3D 건물 내부에 정적/동적 물체를 배치하고, 카메라 및 쉐이딩, 텍스처, 광원 효과를 구현한 프로젝트입니다.
예제 프로그램 `4.9.2.Our_House_V_0.55` 기반으로 기능을 확장하여 다음을 구현했습니다:

* **정적 물체**: ant, cat, iron man, bike, wood tower
* **동적 물체**: wolf, spider (곡선 및 직선 경로 이동)
* **카메라 시스템**:

  * 메인 카메라 (움직임/회전/확대/축소)
  * 정적 CCTV 3개
  * 동적 CCTV 1개 (별도 제어 가능)
  * 도면용 orthographic 카메라 (정면, 상면, 측면)
* **쉐이딩**: Simple / Gouraud / Phong shading 전환 (0/1/2 키)
* **텍스처 매핑**: wood tower, wolf에 직접 촬영한 이미지 적용, 필터 모드 전환 (N/L 키)
* **광원 시스템**: 월드 좌표계 기준, 카메라 기준, spider 고정 광원 on/off
* **투명 효과**: teapot에 투명 모드 및 투명도 조절 (‘6’, ‘+’, ‘-’ 키)

---

## 🌟 구현 화면
<img width="1000" height="1111" alt="스크린샷 2025-07-11 180411" src="https://github.com/user-attachments/assets/bb2ed8eb-58ab-4d7b-818f-77df22df2448" />
<img width="1000" height="1166" alt="스크린샷 2025-07-11 180525" src="https://github.com/user-attachments/assets/863971a9-391b-43c8-a27e-78ed00fe9192" />
<img width="1000" height="1082" alt="스크린샷 2025-07-11 180726" src="https://github.com/user-attachments/assets/0acacfe1-ddf1-4ca0-8664-c1b6ff87b5fa" />

---

## 🛠️ 주요 기능 및 키보드 조작

| 키                | 기능                                  |
| ---------------- | ----------------------------------- |
| q, w, e, r, t, y | U, V, N 축 방향 이동 / 회전 (space로 모드 전환) |
| space            | 이동 ↔ 회전 모드 전환                       |
| 마우스 휠            | 확대 / 축소                             |
| v                | 동적 CCTV 모드 전환                       |
| x                | CCTV RGB 좌표축 표시 토글                  |
| 1, 2, 0          | Gouraud, Phong, Simple 쉐이딩 전환       |
| N, L             | Nearest, Linear 텍스처 필터 전환           |
| 3, 4, 5          | 월드 광원, 카메라 광원, spider 광원 on/off     |
| 6, +, -          | teapot 투명 효과 on/off 및 투명도 조절        |

---

## 🏛️ 프로젝트 구조

* `Camera.h/cpp`: 카메라 정의 및 각종 카메라 뷰포트 초기화
* `Texture_Manager.h/cpp`: FreeImage를 사용한 텍스처 로딩 및 필터 적용
* `Scene_Definitions.h/cpp`: 씬, 물체, 쉐이더, 카메라 데이터 관리
* `Dynamic_Objects.cpp`: wolf, spider의 경로 설정 및 이동 로직
* `Static_Objects.cpp`: 정적 물체의 geometry 및 속성 정의
* `Shaders.cpp`: simple, gouraud, phong 및 텍스처 쉐이더 로드
* `4.9.2.Our_House_V_0.55_main.cpp`: 메인 루프 및 키보드/마우스 이벤트 처리

---

## 💻 빌드 & 실행

1️⃣ **환경 준비**
OpenGL (4.0+), GLEW, GLUT, FreeImage 라이브러리 필요

2️⃣ **컴파일**

```bash
g++ 4.9.2.Our_House_V_0.55_main.cpp *.cpp -lGLEW -lGL -lglut -lfreeimage -o OurHouse
```

3️⃣ **실행**

```bash
./OurHouse
```

---

## 🌟 추가 구현

✅ 곡선 경로를 따라 이동하는 spider, ping-pong 모드 wolf
✅ wood tower, wolf에 직접 촬영한 이미지 텍스처 적용
✅ 다각형 물체(teapot) 투명 효과 및 실시간 투명도 조절
✅ 최대 3개 CCTV + 1 동적 CCTV + orthographic 카메라로 멀티뷰 구성

