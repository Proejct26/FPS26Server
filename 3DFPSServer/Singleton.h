#pragma once

template <typename T>
class SingletonBase {
public:
    // 복사 생성자와 대입 연산자를 삭제하여 복사 방지
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

    // 싱글턴 인스턴스를 얻는 정적 메서드
    static T& GetInstance() {
        static T instance;
        return instance;
    }

protected:
    // 상속을 위해 protected 생성자
    SingletonBase() {
        // 초기화 코드
    }

    // 상속을 위해 protected 소멸자
    virtual ~SingletonBase() {
        // 정리 코드
    }

    // 기타 protected 멤버와 메서드
};