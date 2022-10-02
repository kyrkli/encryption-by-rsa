#include "XBlocks8B.h"
//#include <bits/stdc++.h>
//#include <openssl/rsa.h>
#include <fstream>
#include "my_rsa.h"
#include <map>

using namespace std;

//Sieve of Atkin for finding prime numbers
/*
void sieve_of_atkin(const XBlocks8B& limit) {

	// Initialise the sieve array with false values
	std::map<XBlocks8B, bool> sieve;

	/* Mark sieve[n] is true if one
	of the following is true:
	a) n = (4*x*x)+(y*y) has odd number of
	solutions, i.e., there exist
	odd number of distinct pairs (x, y)
	that satisfy the equation and
		n % 12 = 1 or n % 12 = 5.
	b) n = (3*x*x)+(y*y) has odd number of
	solutions and n % 12 = 7
	c) n = (3*x*x)-(y*y) has odd number of
	solutions, x > y and n % 12 = 11 */
/*
	int j = 0;
	for (XBlocks8B x = limit / 256; (x * x < limit); x++) {
		for (XBlocks8B y = limit / 256; (y * y < limit); y++) {
			// 1 Main part of Sieve of Atkin
			cout << " iterations number = " << ++j << endl;
			XBlocks8B n = (x * x * 4) + (y * y);
			if (n <= limit && (n % 12 == 1 || n % 12 == 5)) {
				if (sieve.contains(n))
					sieve[n] ^= true;
				else sieve[n] = true;
			}

			//2
			n = (x * x * 3) + (y * y);
			if (n <= limit && n % 12 == 7) {
				if (sieve.contains(n))
					sieve[n] ^= true;
				else sieve[n] = true;
			}

			//3
			if ((x * x * 3) >= (y * y)) {
				n = (x * x * 3) - (y * y);

				if (x > y && n <= limit && n % 12 == 11) {
					if (sieve.contains(n))
						sieve[n] ^= true;
					else sieve[n] = true;
				}

			}

		}
	}
	// Mark all multiples of squares as non-prime
	for (XBlocks8B r = 5; r * r < limit; r++) {
		if (sieve[r] == true) {
			for (XBlocks8B i = r * r; i < limit; i += r * r)
				sieve[i] = false;
		}
	}

	int i = 1;
	// Print primes using sieve[]
	for (XBlocks8B a = 5; a < limit; a++)
		if (sieve[a] == true)
			cout << a << " number = " << i++ << endl;

	//TODO with eEa check all numbers

}
*/
//extended Euclidean algorithm
void ext_eucl_alg(int a, int b) {
	std::vector<int> r{ a, b };
	std::vector<int> x{ 1, 0 };
	std::vector<int> y{ 0, 1 };
	std::vector<int> q{ 0, 0 };
	int i = 1;

	while (r[i] > 0) {
		r.push_back(r[i - 1] % r[i]);
		q.push_back(r[i - 1] / r[i]);
		x.push_back(x[i - 1] - q[i + 1] * x[i]);
		y.push_back(y[i - 1] - q[i + 1] * y[i]);
		i++;
	}

	cout << "a = " << a << "; b = " << b << endl << "r[i-1] = " << r[i - 1] << "; x[i-1] = " << x[i - 1] << "; y[i-1] = " << y[i - 1] << endl;
	cout << r[i - 1] << " = " << a << "*" <<
		x[i - 1] << " + " << b << "*" << y[i - 1] << endl;
}

void primeInRange(XBlocks8B L, XBlocks8B R)
{
	XBlocks8B i, j;
	bool flag;

	// Traverse each number in the
	// interval with the help of for loop
	for (XBlocks8B i = L; i <= R; i++) {
		cout << " i = " << i << endl;
		// Skip 0 and 1 as they are
		// neither prime nor composite
		if (i == 1 || i == 0)
			continue;

		// flag variable to tell
		// if i is prime or not
		flag = 1;

		// Iterate to check if i is prime
		// or not

		for (XBlocks8B j = 2; j <= i / 2; j++) {
			//cout << "before i = " << i << " j = " << j << endl;
			if (i % j == 0) {
				//cout << "in i = " << i << " j = " << j << endl;
				flag = 0;
				break;
			}
		}

		// flag = 1 means i is prime
		// and flag = 0 means i is not prime
		if (flag == 1)
			cout << "Result = " << i << endl;
	}
}

int main() {
	XBlocks8B tmp1 = string("16");
	XBlocks8B tmp2 = string("0");
	XBlocks8B tmp3(250, 1000000);
	XBlocks8B tmp4(250, 10110);
	XBlocks8B tmp5(3, 1);
	cout << tmp1 << endl << "////////" << endl << tmp2 << endl << "================" << endl;
	
	XBlocks8B mod("00:bf:ba:b6:ac:fd:da:5b:bd:93:8e:17:0e:d4:16:05:51:2e:75:71:fe:14:34:32:c2:c1:a4:dd:49:12:df:9e:a7:64:9b:d1:cc:86:e3:a2:b7:89:b9:09:fa:59:47:5b:00:c2:8d:50:33:74:31:24:68:cd:c4:c4:82:f9:16:3f:ca:96:bb:4e:88:bf:8f:47:28:60:d1:dd:61:90:1a:82:09:8b:71:21:30:62:97:aa:2d:75:e1:cc:8a:c3:58:b2:e4:c7:7f:02:b1:56:dd:8e:87:0f:dc:f6:48:c8:28:70:72:4c:86:68:d4:92:15:37:31:76:44:07:46:22:7a:dc:d1");
	XBlocks8B pe(65537);
	XBlocks8B pre("6b:9c:b0:cb:87:ef:f7:6e:00:53:d0:3d:52:6c:c9:af:37:14:54:56:f7:a0:14:9d:9b:72:9d:81:56:46:8d:16:07:32:2e:1f:4d:d6:96:8d:cd:8f:3e:3a:75:c1:c7:22:36:10:7a:e3:66:63:ee:bb:a4:42:bc:c9:86:65:26:87:e3:b8:48:15:58:37:79:5c:94:5d:1b:66:be:1b:cb:47:4b:5d:ec:bc:61:4d:ab:c7:fe:a8:94:df:8c:dc:8a:cb:7f:20:b0:3d:8f:66:36:40:05:fd:51:90:9c:59:7c:5b:60:9a:33:18:97:b4:69:3d:db:59:25:d3:6f:6d:00:01");

	my_rsa tmp_rsa(mod, pe, pre);
	auto encoded = tmp_rsa.encode('a');
	cout << " RESULT encoded = " << encoded << endl;
	cout << " RESULT decoded = " << tmp_rsa.decode(encoded) << endl;
	cout << " END OF PROGRAM " << endl;
	return 0;

}
