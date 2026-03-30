#include "agent/Agent.h"
#include "config/ConfigManager.h"
#include <thread>
#include <chrono>
#include <logging/Logger.h>

using json = nlohmann::json;

Agent::Agent(Config cfg)
    : config(cfg), http(cfg.server_url) {
    spdlog::info("Agent created. UID={}", config.uid);
}

void Agent::registerAgent()
{
    json req;
    req["UID"] = config.uid;
    req["descr"] = config.descr;

    spdlog::debug("Starting agent registration. Registration request: {}", req.dump());

    json response = http.post("/wa_reg/", req);

    spdlog::info("Register response: {}", response.dump());

    if (response.contains("access_code")) {
        spdlog::info("Received new access_code from server");
        const std::string key = "access_code";
        const std::string value = response["access_code"];
        
        spdlog::info("Updating config: {}", key);
        ConfigManager cm;
        cm.setValue("config.json", key, value);
        config = cm.load("config.json");

        spdlog::info("Config successfully updated: {}", key);
	}

    return;
}

void Agent::processCycle() {
    spdlog::debug("Starting processing cycle");
    
    json req = {
        {"UID", config.uid},
        {"access_code", config.access_code}
    };

    spdlog::debug("Requesting tasks from server");
    auto resp = http.post("/wa_task/", req);

    if (resp.empty()) {
        spdlog::debug("No response or empty response from server");
        return;
    }

    if (resp.value("code_responce", "0") == "0") {
        spdlog::debug("No task from the server");
        return;
    }
    
    if (resp.value("code_responce", "0") == "-12") {
        spdlog::debug("Agent not registered");
        registerAgent();
        return;
    }

    spdlog::debug("Parsing task from response");
    Task task(resp);

    spdlog::debug("Executing task");
    json exec_result = executor.execute(task, config);
    spdlog::debug("Execution result: {}", exec_result.dump());

	int result_code = exec_result.value("code_response", -1);

	std::vector<std::string> files;
    if (exec_result.contains("file_names")) {
        const std::string base_dir = config.result_directory;

        for (const auto& name : exec_result["file_names"]) {
            files.emplace_back(base_dir + "/" + name.get<std::string>());
        }

        exec_result.erase("file_names");
    }

	json result_payload = {
        {"UID", config.uid},
        {"access_code", config.access_code},
        {"message", exec_result.value("message", "")},
        {"files", files.size()},
        {"session_id", task.getSessionID()}
    };

	spdlog::debug("Final result payload: {}", result_payload.dump());


	json post_response = http.postMultipart(
        "/wa_result/",
        result_code,
        result_payload,
        files
    );

	spdlog::info("Server response: {}", post_response.dump());
}

void Agent::run() {
    spdlog::info("Agent started");
    while (true) {
        spdlog::debug("New iteration of main loop");
        processCycle();
        std::this_thread::sleep_for(std::chrono::seconds(config.poll_interval_sec));
    }
}