#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace Domain
{
    bool is_Subdomain(const string& domain, const string& subdomain)
    { // проверка на поддомен
        int64_t start_pos = subdomain.size() - domain.size();
        if (start_pos < 0)
        {
            return false;
        }

        if (subdomain.find(domain) == 0 && (start_pos == 0 || subdomain[domain.size()] == '.'))
        {
            return true;
        }
        return false;
    }

    vector<string> Read(istream& in = cin)
    { 
        size_t count;
        in >> count;
        in.ignore(1);

        vector<string> domains;
        string domain;
        for (size_t i = 0; i < count && getline(in, domain); ++i) {
            if (!domain.empty())
            {
                reverse(begin(domain), end(domain));
                domains.push_back(move(domain));
            }
        }
        return domains;
    }

    vector<string> BuildBanned(istream& in = cin)
    { // считать запрещенные домены
        vector<string> domains = Read(in);

        sort(begin(domains), end(domains));
        auto end_it = unique(begin(domains), end(domains), is_Subdomain);

        domains.erase(end_it, end(domains));
        domains.shrink_to_fit();
        return domains;
    }

    class Checker
    {
        vector<string> banned_domains;
        vector<string> domains_to_check;
    public:
        Checker(istream& in = cin) : banned_domains(BuildBanned(in)), domains_to_check(Read(in)) {}

        void Print(std::ostream& out = cout) const
        {
            for (const string& domain : domains_to_check)
            {
                const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
                if (
                    it != begin(banned_domains) && is_Subdomain(*prev(it), domain)) {
                    out << "Bad" << endl;
                }
                else {
                    out << "Good" << endl;
                }
            }
        }
        // Getters , Setters
    };
}

int main() {
    cin.tie(nullptr);
    ios_base::sync_with_stdio(false);

    Domain::Checker domains;
    domains.Print();
    return 0;
}
