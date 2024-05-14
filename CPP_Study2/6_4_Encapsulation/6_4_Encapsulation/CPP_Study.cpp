#include <iostream>
using namespace std;

// ������ ���� : ���м�

// ��ü���� (OPP : Object Oriented Programming)
// - ��Ӽ�
// - ���м� = ĸ��ȭ
// - ������

// ���м�(Data Hiding) = ĸ��ȭ(Encapsulation)
// ���� �Ǵ� ���� ����ϰ� ����ڴ�!
// ����� ����?
// - 1) ���� �����ϰ� �ǵ帮�� �ȵǴ� ���
// - 2) �ٸ� ��η� �����ϱ� ���ϴ� ���

// �ڵ���
// - �ڵ�
// - ���
// - ����
// - ��
// - ���� ���⼱

// �Ϲ� ������ ���忡�� ����ϴ� ��?
// - �ڵ�/���/��
// ���� �� (������ �ǵ帮�� ū�ϳ�0
// - ����, ���� ���⼱

// public (������?)  protected (��ȣ�޴�?)  private (������?)
// - public : �������׳� ����. ������ ��� ����
// - protected : ���� �ڼյ����׸� ���
// - private : ���� ����� ��. << class Car ���ο�����!

// ��� ���� ������ : ���� �������� �θ���� ������ ��� ��������?
// �θ������ �������� ������ �� ���� �ڼյ����׵� �Ȱ��� ������� ������ ����.
// - public : ������ ���? �θ���� ������ ���� �״�� - (public -> public, protected -> protected)
// - protected : ��ȣ�޴� ���? �� �ڼյ� ���׸� �����ٰ���. (public -> protected, protected -> protected)
// - private : �������� ���? �������� �� ���� �ڼյ����״� �������� �ʰڴ�. (public->private, protected->private)

// ��ǻ� ��� ���� �����ڴ� public �� �� ���� ���� ���̴�.

class Car
{
public: // (���) ���� ������
	void MoveHandle() {}
	void PushPedal() {}
	void OpenDoor() {}

	void TurnKey()
	{
		// ...
		RunEngine();
	}

protected:
	void DisassembleCar() { } // ���� �����Ѵ�
	void RunEngine() { } // ������ ������Ų��
	void ConnectCircuit() { } // ���⼱ ����

public:
	// �ڵ�
	// ���
	// ����
	// ��
	// ���� ���⼱

};

class SuperCar : private Car // ��� ���� ������
{
public:
	void PushRemoteController()
	{
		RunEngine();
	}
};

class TestSuperCar : SuperCar // class TestSuperCar : private SuperCar�� ����. �� �������� ������ private�� �ȴ�.
{
public:
	void Test()
	{
		//DisassembleCar(); // ��� �Ұ� (Car�� ��� ���� �����ڰ� private�� �����Ǿ� �ֱ� ����

	}
};

// 'ĸ��ȭ'
// ������ �����Ϳ� �Լ��� �������� ������� ��
class Berserker
{
public:
	// ��� : ü���� 50 ���Ϸ� �������� ����Ŀ ��� �ߵ�(������)
	int GetHp() { return _hp; }
	void SetHp(int hp)
	{
		_hp=hp;
		if (_hp <=50)
			SetBerserkerMode();
	}
private:
	void SetBerserkerMode()
	{
		cout << "�ſ� ������!" << endl;
	}

private:
	int _hp=100;
};

int main()
{
	Berserker b;
	b.SetHp(20);
	

	return 0;
}

// ��, �׼��� ������ ������ �߻��ϸ� ��� ���� ������, ���� �����ڿ� ���� Ȯ���� ���ƾ� �Ѵ�.