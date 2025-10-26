#include<iostream>
#include <cmath>   
#include <algorithm> 
using namespace std;
class Polynomial;
class Term {
	friend Polynomial;
public:
	float coef;
	int exp;
};
class Polynomial {
private:
	Term* termArray;
	int capacity;
	int terms;
public:
	Polynomial() :capacity(2), terms(0) {
		termArray = new Term[capacity];
	}

	~Polynomial() { 
		delete[] termArray;
	}
	void look(int n) {
		if (capacity >= n)return;
		int c = max(capacity * 2, n);
		Term* temp = new Term[c];
		copy(termArray, termArray + terms, temp);
		delete[]termArray;
		termArray = temp;
		capacity = c;
	}
	Polynomial Add(const Polynomial &poly) {
		Polynomial result;
		for (int i = 0; i < terms; i++) {
			result.newTerm(termArray[i].coef, termArray[i].exp);
		}
		for (int j = 0; j < poly.terms; j++) {
			int exp = poly.termArray[j].exp;
			double coef = poly.termArray[j].coef;
			int find = 0;
			for (int i = 0; i < result.terms; i++) {
				if (result.termArray[i].exp == exp)
				{
					result.termArray[i].coef += coef;
					find = 1;
				}
			}
			if (find == 0 && coef != 0) {
				result.look(result.terms + 1);
				result.termArray[result.terms].coef = coef;
				result.termArray[result.terms].exp = exp;
				result.terms++;
			}
		}
		return result;
	}
	Polynomial mult(const Polynomial &poly) {
		Polynomial result;
		for (int i = 0; i < terms; i++) {
			for (int j = 0; j < poly.terms; j++) {
				float coef = termArray[i].coef * poly.termArray[j].coef;
				int exp = termArray[i].exp + poly.termArray[j].exp;
				int find = 0;
				for (int k = 0; k < result.terms; k++) {
					if (result.termArray[k].exp == exp) {
						result.termArray[k].coef += coef;
						find = 1;
					}
				}
				if (!find && coef != 0) {
					result.look(result.terms + 1);
					result.termArray[result.terms].coef = coef;
					result.termArray[result.terms].exp = exp;
					result.terms++;
				}
			}
		}
		return result;
	}
	float  Eval(float f) {
		float sum = 0;
		for (int i = 0; i < terms; i++) {
			sum += termArray[i].coef * pow(f, termArray[i].exp);
		}
		return sum;
	}
	void newTerm(const float newcoef, int newexp) {
		if (newcoef == 0)return;
		if (terms == capacity) {
			capacity *= 2;
			Term* temp = new Term[capacity];
			copy(termArray, termArray + terms, temp);
			delete[]termArray;
			termArray = temp;
		}
		termArray[terms].coef = newcoef;
		termArray[terms].exp = newexp;
		terms++;
	}
	friend istream& operator>>(istream& in, Polynomial& poly);
	friend ostream& operator<<(ostream& out, const Polynomial& poly);
};

istream& operator>>(istream& in, Polynomial& poly) {
	float coef;
	int exp;
	int n;
	cin >> n;
	poly.terms = 0;
	while (n--) {
		in >> coef >> exp;
		poly.newTerm(coef, exp);
	}
	return in;
}
ostream& operator<<(ostream& out, const Polynomial& poly) {
	for (int i = 0; i < poly.terms; i++) {
		if (poly.termArray[i].coef < 0 && i>0)out << "-";
		else if (poly.termArray[i].coef > 0 && i > 0)out << "+";
		out << poly.termArray[i].coef;
		if (poly.termArray[i].exp != 0)out << "X^" << poly.termArray[i].exp;
	}
	return out;
}
int main() {
	Polynomial p1, p2;
	cin >> p1 >> p2;
	Polynomial sum = p1.Add(p2);
	Polynomial p = p1.mult(p2);
	float x;
	cin >> x;
	cout << "P1+P2=" << sum << endl;
	cout << "P1*P2=" << p << endl;
	cout << "P1(x)=" << p1 << endl;
	cout << "P1(" << x << ")=" << p1.Eval(x) << endl;
	return 0;
}
