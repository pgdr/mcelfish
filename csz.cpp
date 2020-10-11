#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <assert.h>
#include <random>


float
_weighted_sum (std::vector < int >data)
{
    float s = 0;
    for (int i = 0; i < data.size (); i++)
        s += ((data.size () - (1 + i)) * data[i]);
    return s;
}

int
sum (std::vector < int >data)
{
    int s = 0;
    for (int i = 0; i < data.size (); i++)
        s += data[i];
    return s;
}




float
_z_pre_estimate (std::vector < int >data, int hatN)
{
    auto t = sum (data);
    auto x = _weighted_sum (data);
    auto k =  data.size();

    auto tellerA = hatN - t + 0.5;
    auto tellerB = pow ((k * hatN - x), k);
    auto nevner = pow ((k * hatN - x - t), k);

    return (tellerA * tellerB) / nevner - 0.5;
}

float
_cs_Eq (int t, int hatN, int k, float x)
{
    float prod = 1;
    for (int i = 0; i < k; i++) {
        int j = i + 1;
        auto teller = k * hatN - x - t + 1 + k - j;
        auto nevner = k * hatN - x + 2 + k - j;
        prod *= (teller * 1.0) / nevner;
    }
    return t - 1 + ((hatN + 1) * prod);
}

float
removal_carle_strub (std::vector < int >data)
{
    float t = sum (data);
    float x = _weighted_sum (data);
    int k =  data.size();
    int hatN = t;

    for (int i = 0; i < 1000 * 1000; i++) {
        auto lhs = hatN + i;
        auto rhs = _cs_Eq (t, lhs, k, x);
        if (lhs >= rhs)
            return lhs;
    }
    throw "Unable to find CS solution";
}

float
removal_zippin (std::vector < int >data)
{
    int t = sum (data);
    int k =  data.size();
    float x = _weighted_sum (data);
    auto z_min = ((t - 1) * (k - 1) / 2) - 1;

    if (x <= z_min)
        throw "Zippin X below z_min for {data}";

    int hatN = t;
    for (int i = 0; i < 1000 * 1000; i++) {
        auto lhs = hatN + i;
        auto rhs = _z_pre_estimate (data, lhs);
        if (rhs > lhs)
            return lhs;
    }
    throw "Unable to find CS solution";
}


void
t1 ()
{
    std::vector < int >data {
        100, 10, 1, 1, 0, 0 };
    auto hat_nz = 112;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 112;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);
}

void
t2 ()
{
    std::vector < int >data {
        19, 17, 13, 1, 1 };
    auto hat_nz = 53;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 53;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}


void
t3 ()
{
    std::vector < int >data {
        10, 20, 30, 24, 2, 7 };
    auto hat_nz = 156;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 145;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}

void
t4 ()
{
    std::vector < int >data {
        5, 1, 0, 0, 0, 0, 0 };
    auto hat_nz = 6;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 6;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}

void
t5 ()
{
    std::vector < int >data {
        34, 46, 22, 26, 18, 16, 20, 12 };
    auto hat_nz = 268;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 264;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}

void
t6 ()
{
    std::vector < int >data {
        32, 40, 12, 19, 9, 7, 8, 5, 2, 3, 1, 1, 0 };
    auto hat_nz = 141;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 140;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}

void
t7 ()
{
    std::vector < int >data {
        22, 9, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0 };
    auto hat_nz = 38;
    auto act_nz = removal_zippin (data);
    assert (hat_nz == act_nz);
    auto hat_ncs = 38;
    auto act_ncs = removal_carle_strub (data);
    assert (hat_ncs == act_ncs);

}

int
main ()
{
    t1 ();
    t2 ();
    t3 ();
    t4 ();
    t5 ();
    t6 ();
    t7 ();

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0, 1);

    std::vector < int >data {};

    float p = 0.3;
    int N = 200;
    int N_curr = N;
    while ((N_curr > N * 0.2 && data.size() < 30)) {
        int c = 0;
        for (int i = 0; i < N_curr; i++) {
            if (dist(e2) < p)
                c += 1;
        }
        N_curr -= c;
        data.push_back(c);

    }

    std::cout << "N = " << N << ", p = " << p << std::endl;
    for (int i = 0; i < data.size(); i++)
        std::cout << data[i] << " ";
    std::cout << std::endl;

    auto z = removal_zippin (data);
    std::cout << "Zippin      " << z << std::endl;

    auto cs = removal_carle_strub (data);
    std::cout << "Carle Strub " << cs << std::endl;

    return 0;
}
