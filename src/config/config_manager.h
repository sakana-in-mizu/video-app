#pragma once

#include <filesystem>
#include <map>
#include <string>

#include "base/singleton.h"

class ConfigManager : public Singleton<ConfigManager> {
    friend Singleton<ConfigManager>;

public:
    std::filesystem::path getConfigFilePath() const { return m_config_file_path; }

    void setConfigFilePath(const std::filesystem::path &path) { m_config_file_path = path; }

    void parse();

    std::string getValue(const std::string &section, const std::string &key) const;

private:
    std::filesystem::path m_config_file_path {};

    std::map<std::string, std::map<std::string, std::string>> m_result;

    ConfigManager();
};