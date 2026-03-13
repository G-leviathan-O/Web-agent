#include "Agent.h"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

Agent::Agent(const Config& cfg)
    : config(cfg),
      http(cfg.getServerURL())
{
    spdlog::info("Agent created: {}", cfg.getUID());
}

Config Agent::getConfig() const
{
    return config;
}

void Agent::updateConfig(Config cfg)
{
	config = cfg;
}

void Agent::registerAgent()
{
    json req;
    req["UID"] = config.getUID();
    req["descr"] = config.getDescr();

    spdlog::debug("Starting agent registration. Registration request: {}", req.dump());

    json response = http.post("/wa_reg/", req);

    spdlog::info("Register response: {}", response.dump());

    if (response.contains("access_code")) {
        spdlog::info("Received new access_code from server");
        config.setAccessCode(response["access_code"]);
	}

    return;
}

void Agent::pollTasks()
{
    spdlog::debug("Polling tasks from server.");

    json req;

    req["UID"] = config.getUID();
    req["descr"] = config.getDescr();
    req["access_code"] = config.getAccessCode();

    spdlog::debug("Task request: {}", req.dump());

    json response = http.post("/wa_task/", req);

    spdlog::debug("Task response: {}", response.dump());

	if(response.empty()) {
        spdlog::debug("Server returned empty response"); return;
    }
	if(response["code_responce"] == "0") {
        spdlog::debug("No tasks available"); return;
    }
    if(response["code_responce"] == "-12") {
        spdlog::warn("Server reported agent not registered");
        throw std::runtime_error("Agent not registered");
    }

    Task task(response);
	TaskExecutor executor(config.getResultDirectory());

    spdlog::info("Executing task...");
    auto start_time = std::chrono::high_resolution_clock::now();

	json result = executor.execute(task, this);

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    result["UID"] = config.getUID();
    result["access_code"] = config.getAccessCode();
    result["session_id"] = task.getSessionID();

	spdlog::info("Task executed in {:.3f} seconds", elapsed.count());
    spdlog::debug("Task result: {}", result.dump(4));

    json post_response = http.post("/wa_result/", result);
	spdlog::info("Server response to result post: {}", post_response.dump(4));
}

void Agent::run()
{
    spdlog::info("Agent main loop started");

    while(true)
    {
        try
        {
            pollTasks();
        }
        catch(const std::runtime_error& e)
        {
            spdlog::warn("Re-registering agent: {}", e.what());
            registerAgent();
        }
        catch(const std::exception& e)
        {
            spdlog::error("Agent error: {}", e.what());
        }

        unsigned int interval = config.getPollInterval();

        spdlog::debug("Sleeping for {} seconds", interval);

        std::this_thread::sleep_for(
            std::chrono::seconds(interval)
        );
    }
}