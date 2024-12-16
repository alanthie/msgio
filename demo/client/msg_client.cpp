#include "proto_client.h"
#include <thread>
#include <iostream>
#include <stdlib.h>
#include <chrono>

int main(int /*argc*/, char** /*argv*/)
{
    //ulimit -n 2024
    const size_t N = 400;
    size_t n = 0;
    size_t n_bytes = 0;
    std::string test_message = "TEST_MESSAGETEST_MESSAGETEST_MESSAGETEST_MESSAGE";
    for (size_t i=0;i< 5; i++)
      test_message += test_message;
    std::chrono::steady_clock::time_point tbegin = std::chrono::steady_clock::now();

    std::vector<proto::msgclient*> vc;
    for (size_t i=0;i< N; i++)
    {
      proto::msgclient* client = new proto::msgclient();
      vc.push_back(client);
      client->set_message_callback([&](proto::msgclient&, const void* buffer, size_t num)
      {
          n++;
          n_bytes+=num;
      });

      // Run on port 9000
      client->set_target("127.0.0.1:9000");
      client->start();
    }

    // Run for 5 minutes
    auto start_tp = std::chrono::steady_clock::now();
      
while (std::chrono::steady_clock::now() - start_tp < std::chrono::minutes(5) )
{
    for (size_t i=0;i< N; i++)
    {
      {
          vc[i]->send_msg(test_message.c_str(), test_message.size());
          vc[i]->update();     
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    
    if (n% 10000 == 0)
    {
      std::chrono::steady_clock::time_point tend = std::chrono::steady_clock::now();
      float rate = ((float)n) / (0.01+(float)std::chrono::duration_cast<std::chrono::seconds>(tend - tbegin).count());
      float brate = ((float)n_bytes) / (0.01+(float)std::chrono::duration_cast<std::chrono::seconds>(tend - tbegin).count());

      std::cout <<  std::to_string(N) << " clients, Message in+out rate msg/sec = " << 2*rate << std::endl;
      std::cout <<  std::to_string(N) << " clients, Bytes in+out rate bytes/sec = " << 2*brate << std::endl;
    }
}
    
    for (size_t i=0;i< N; i++) delete vc[i];
    return EXIT_SUCCESS;
}
