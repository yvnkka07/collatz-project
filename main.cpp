#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <mutex>
#include <atomic>
#include <chrono>

using namespace std;

const long long N = 10000000;

atomic<long long> current(1);
long long totalStepsSum = 0;
mutex sumMutex;

// Function to compute Collatz steps
long long collatzSteps(long long n) {
    long long steps = 0;
    while (n != 1) {
        if (n % 2 == 0)
            n /= 2;
        else
            n = 3 * n + 1;
        steps++;
    }
    return steps;
}

void worker() {
    long long localSum = 0;
    while (true) {
        long long num = current.fetch_add(1);
        if (num > N) break;

        localSum += collatzSteps(num);
    }

    lock_guard<mutex> lock(sumMutex);
    totalStepsSum += localSum;
}

int main() {
    unsigned int numThreads = thread::hardware_concurrency();
    if (numThreads == 0) numThreads = 4;

    cout << "Using threads: " << numThreads << endl;

    vector<thread> threads;

    auto start = chrono::high_resolution_clock::now();

    for (unsigned int i = 0; i < numThreads; i++) {
        threads.emplace_back(worker);
    }

    for (auto &t : threads) {
        t.join();
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;

    double averageSteps = (double)totalStepsSum / N;

    ofstream file("results.txt");
    file << "Execution time: " << duration.count() << " seconds\n";
    file << "Total steps: " << totalStepsSum << "\n";
    file << "Average steps: " << averageSteps << "\n";
    file.close();

    cout << "Execution time: " << duration.count() << " seconds" << endl;
    cout << "Average steps: " << averageSteps << endl;

    return 0;
}
