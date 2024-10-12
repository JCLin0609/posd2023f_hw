#include <iostream>
#include <iostream>
#include <string>
// #include <algorithm>

bool comp(const std::string &a, const std::string &b)
{
    return a < b;
}

int main()
{
    std::vector<std::string> _sortedNodes;
    _sortedNodes.push_back("Downloads");
    _sortedNodes.push_back("Documents");
    _sortedNodes.push_back("my_profile");

    std::sort(_sortedNodes.begin(), _sortedNodes.end(), comp);

    std::cout << _sortedNodes[0];
}