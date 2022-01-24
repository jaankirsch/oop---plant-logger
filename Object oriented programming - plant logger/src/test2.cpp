// #include <iostream>
// #include <thread>

// static bool is_Finished = false;

// void DoWork()
// {
//     while (!is_Finished)
//         {
//         std::cout << "Working.... \n";
//         }
//     }

// void lamp()
// {
//     std::thread worker(DoWork);

//     std::cin.get();
//     is_Finished = true;

//     worker.join();

//     std::cin.get();
// }
