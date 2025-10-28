## TBI ( The Binding of Isaac )  ([상세내용](https://tobrother.tistory.com/category/WinApi/TBI%28%EB%8D%94%20%EB%B0%94%EC%9D%B8%EB%94%A9%20%EC%98%A4%EB%B8%8C%20%EC%95%84%EC%9D%B4%EC%9E%91%29%20%EB%AA%A8%EC%9E%91))

> ( 2025.03 ~ 2025.05 ) 1인 개발


### 사용 기술 및 언어
- C++, Win32API, DirectWrite, Direct2D, GDI+, FMOD


#### 핵심 시스템
- **플레이어 시스템**: 캐릭터 이동, 공격, 체력 관리 
- **맵 시스템**: 로그라이크 방식의 랜덤 맵 생성 ([상세내용](https://tobrother.tistory.com/134))
- **적 시스템**: 다양한 몬스터 AI 및 행동 패턴 ([상세내용](https://tobrother.tistory.com/139))
- **아이템 시스템**: 픽업 아이템(동전, 폭탄, 열쇠) 및 액티브, 패시브 아이템
- **방 시스템**: 일반 방, 황금 방, 보스 방 등 다양한 방 타입

#### 게임 요소
- 엔티티 관리 시스템
- 씬 전환 시스템
- 충돌 처리

## 📂 프로젝트 구조

```
TBI/
├── Engine/              # 핵심 게임 엔진
│   ├── 01. Header/      # 헤더파일
│   ├── 02. Core/        
│   ├── 03. SingleTon/   # 싱글톤
│   ├── 04. Scene/       # 씬 관리
│   ├── 05. Object/      # 오브젝트
│   ├── 06. Component/   # 충돌체, UI, 애니메이션 컴포넌트
│   ├── 07. Module/      # 몬스터 AI 관리
│   └── 08. Factory/     # 몬스터 생성
├── resources/           # 게임 리소스
│   ├── font/
│   ├── json/            # 몬스터 배치 정보
│   ├── texture/         
│   └── sound/
└── README.md
```
