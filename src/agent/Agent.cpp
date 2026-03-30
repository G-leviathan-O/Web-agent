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

    json request = {
        {"UID", config.getUID()},
        {"descr", config.getDescr()},
        {"access_code", config.getAccessCode()}
    };

    spdlog::debug("Request payload: {}", request.dump(4));


    json response = http.post("/wa_task/", request);

    spdlog::debug("Response payload: {}", response.dump(4));


	if (response.empty()) {
        spdlog::debug("Empty response from server");
        return;
    }

    const std::string code = response.value("code_responce", "");

	if (code == "0") {
        spdlog::debug("No tasks available");
        return;
    }
    if (code == "-12") {
        spdlog::error("Agent not registered");
        throw std::runtime_error("Agent not registered");
    }


    Task task(response);
	TaskExecutor executor(config.getResultDirectory());

    spdlog::info("Executing task...");
    const auto start_time = std::chrono::high_resolution_clock::now();

	json exec_result = executor.execute(task, this);

    const auto end_time = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double> elapsed = end_time - start_time;

	spdlog::info("Task executed in {:.3f} sec", elapsed.count());
    spdlog::debug("Execution result: {}", exec_result.dump(4));


	int result_code = exec_result.value("code_responce", -1);

	std::vector<std::string> files;
    if (exec_result.contains("file_names")) {
        const std::string base_dir = config.getResultDirectory();

        for (const auto& name : exec_result["file_names"]) {
            files.emplace_back(base_dir + "/" + name.get<std::string>());
        }

        exec_result.erase("file_names");
    }

	json result_payload = {
        {"UID", config.getUID()},
        {"access_code", config.getAccessCode()},
        {"message", exec_result.value("message", "")},
        {"files", files.size()},
        {"session_id", task.getSessionID()}
    };

	spdlog::debug("Final result payload: {}", result_payload.dump(4));


	json post_response = http.postMultipart(
        "/wa_result/",
        result_code,
        result_payload,
        files
    );

	spdlog::info("Server response: {}", post_response.dump(4));
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