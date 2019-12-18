#include "demographic_indicators.h"
#include <iostream>

using namespace std;

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false); // ускоряем ввод-вывод
    DemographicIndicators data;
    data.ReadPeople(cin);

    for (string command; cin >> command; )
    {
        if (command == "AGE") {
            int adult_age;
            cin >> adult_age;

            cout << "There are " << data.AgeCount(adult_age)
                << " adult people for maturity age " << adult_age << '\n';
        }
        else if (command == "WEALTHY") {
            int count;
            cin >> count;

            cout << "Top-" << count << " people have total income " << data.WealtyCount(count) << '\n';
        }
        else if (command == "POPULAR_NAME") {
            char gender;
            cin >> gender;
            string top_name = data.TopName(gender == 'M');

            if (!top_name.empty())
            {
                cout << "Most popular name among people of gender " << gender << " is "
                    << top_name << '\n';
            }
            else
            {
                cout << "No people of gender " << gender << '\n';
            }
        }
    }
}
