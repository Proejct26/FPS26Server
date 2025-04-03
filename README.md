# FPS26Server

**FPS26Server**는 멀티플레이 3D FPS 게임인 **FPS26**의 전용 백엔드 서버 애플리케이션입니다.  
이 서버는 클라이언트와의 실시간 통신을 처리하고, 플레이어의 이동, 공격, 피격, 사망 등 핵심 게임 로직을 서버에서 authoritative하게 처리합니다.

FPS 장르의 특성상 빠르고 정확한 위치 동기화, 상태 변경, 충돌 처리 등이 필요하며, 본 서버는 이를 위해 최적화된 구조로 설계되었습니다.  
또한 Google Protocol Buffers를 사용하여 네트워크 메시지를 경량화하고 언어 간(예: C++ ↔ C#) 통신이 가능하도록 구성되어 있습니다.

---

## ✅ 주요 기능

- **실시간 멀티플레이 지원**  
  클라이언트 간의 위치 동기화 및 상호작용을 서버에서 처리

- **권위적 서버 구조(Authoritative Server)**  
  모든 게임 로직은 클라이언트가 아닌 서버에서 검증 및 실행

- **Protocol Buffers 기반 메시지 직렬화**  
  C++ 및 C# 클라이언트와의 통신을 위한 경량화된 데이터 포맷 사용

- **자동 코드 생성 도구 제공**  
  `.proto` 파일을 기반으로 C++, C# 코드 자동 생성 도구 포함

- **확장 가능한 서버 구조**  
  다양한 기능(예: 팀 매칭, 게임 룸 분리, 점수판 등)으로 확장 가능하도록 설계

---

## 📦 프로젝트 구조

```bash
FPS26Server/
├── 3DFPSServer/                  # 서버 메인 로직 (C++ 기반)
├── AllInOneGenerator/           # 자동화된 코드 생성 스크립트
├── Libraries/                   # 외부 라이브러리 포함 폴더
├── ProtoFileGenerator/          # .proto 파일 자동 생성 도구
├── ProtoToCPPFileGenerator/     # .proto → C++ 코드 생성기
├── ProtoToCSharpFileGenerator/  # .proto → C# 코드 생성기
├── Protobuf/                    # Protobuf 메시지 정의 파일
└── README.md
```

---

## ⚙️ 사용 기술

- **C++17**  
  서버 성능 최적화를 위한 메인 로직 구현

- **Python 3.x**  
  `.proto` 자동 생성 스크립트 및 CLI 툴

- **Protocol Buffers (Protobuf)**  
  네트워크 메시지 직렬화 및 언어 간 메시지 포맷 공유

- **CMake**  
  C++ 프로젝트의 크로스 플랫폼 빌드 시스템

- **C# (Unity Client 연동용)**  
  Unity 클라이언트와의 통신을 위한 메시지 포맷 자동 생성

---

## 🚀 빠른 시작

### 1. 저장소 클론
```bash
git clone https://github.com/Proejct26/FPS26Server.git
cd FPS26Server
```

### 2. 프로토콜 버퍼 설치 (Ubuntu 예시)
```bash
sudo apt install protobuf-compiler
```

### 3. 프로토콜 코드 생성
```bash
cd ProtoFileGenerator
python generate_all.py
```

### 4. 서버 빌드 및 실행
```bash
cd 3DFPSServer
mkdir build && cd build
cmake ..
make
./FPS26Server
```

> C++17 이상, `protobuf` 설치 필요

---

## 👥 기여자

- [ChinoDaiski](https://github.com/ChinoDaiski)
- [znlsnel](https://github.com/znlsnel)

---

## 🤝 기여 방법

Pull Request 및 Issue 등록 환영합니다!  
기여 전 `Proto` 파일 수정 시, 반드시 자동 생성기(`generate_all.py`)를 실행해주세요.

---

## 📄 라이선스

이 프로젝트는 MIT 라이선스 하에 배포됩니다.  
자세한 내용은 [LICENSE](LICENSE) 파일을 참고해주세요.
