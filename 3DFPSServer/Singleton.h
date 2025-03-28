#pragma once

template <typename T>
class SingletonBase {
public:
    // ���� �����ڿ� ���� �����ڸ� �����Ͽ� ���� ����
    SingletonBase(const SingletonBase&) = delete;
    SingletonBase& operator=(const SingletonBase&) = delete;

    // �̱��� �ν��Ͻ��� ��� ���� �޼���
    static T& GetInstance() {
        static std::unique_ptr<T> instance; // ������ �ν��Ͻ��� �����ϴ� unique_ptr
        static std::once_flag initFlag; // �ν��Ͻ� �ʱ�ȭ�� �����ϱ� ���� flag

        std::call_once(initFlag, []() {
            instance.reset(new T()); // �ν��Ͻ� ����
            });

        return *instance; // �ν��Ͻ� ��ȯ
    }

protected:
    // ����� ���� protected ������
    SingletonBase() {
        // �ʱ�ȭ �ڵ�
    }

    // ����� ���� protected �Ҹ���
    virtual ~SingletonBase() {
        // ���� �ڵ�
    }

    // ��Ÿ protected ����� �޼���
};