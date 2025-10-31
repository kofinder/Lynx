#pragma once
#include <string>
#include <stdexcept>
#include <algorithm>

enum class ProjectKind {
    Generic,        // Default / simple project

    Web,            // Web applications (frontend/backends)

    Console,        // CLI tools, terminal apps

    Library,        // Reusable library project

    Service,        // Background service / daemon

    Mobile,         // Mobile apps (iOS/Android)

    Game,           // Game projects (2D/3D)

    Plugin,         // Plugin for another application or editor

    Test,           // Unit test or integration test project

    Microservice,   // Microservice for distributed systems

    DataPipeline,   // Data processing / ETL pipelines

    REST,           // REST API backend project

    WebFlex,        // Frontend framework / SPA project

    GraphQL,        // GraphQL API project
    
    Gateway         // API gateway or micro-gateway project
};

inline ProjectKind operator++(ProjectKind& kind, int) {
    using IntType = typename std::underlying_type<ProjectKind>::type;
    kind = static_cast<ProjectKind>(static_cast<IntType>(kind) + 1);
    return kind;
}

inline ProjectKind parseStringToProjectKind(const std::string &name) {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);

    if (lowerName == "generic") return ProjectKind::Generic;
    if (lowerName == "web") return ProjectKind::Web;
    if (lowerName == "console") return ProjectKind::Console;
    if (lowerName == "library") return ProjectKind::Library;
    if (lowerName == "service") return ProjectKind::Service;
    if (lowerName == "mobile") return ProjectKind::Mobile;
    if (lowerName == "game") return ProjectKind::Game;
    if (lowerName == "plugin") return ProjectKind::Plugin;
    if (lowerName == "test") return ProjectKind::Test;
    if (lowerName == "microservice") return ProjectKind::Microservice;
    if (lowerName == "datapipeline") return ProjectKind::DataPipeline;
    if (lowerName == "rest") return ProjectKind::REST;
    if (lowerName == "webflex") return ProjectKind::WebFlex;
    if (lowerName == "graphql") return ProjectKind::GraphQL;
    if (lowerName == "gateway") return ProjectKind::Gateway;

    throw std::invalid_argument("Unknown project type: " + name);
}

inline std::string parseProjectKindToString(ProjectKind kind) {
    switch (kind) {
        case ProjectKind::Generic: return "generic";
        case ProjectKind::Web: return "web";
        case ProjectKind::Console: return "console";
        case ProjectKind::Library: return "library";
        case ProjectKind::Service: return "service";
        case ProjectKind::Mobile: return "mobile";
        case ProjectKind::Game: return "game";
        case ProjectKind::Plugin: return "plugin";
        case ProjectKind::Test: return "test";
        case ProjectKind::Microservice: return "microservice";
        case ProjectKind::DataPipeline: return "datapipeline";
        case ProjectKind::REST: return "rest";
        case ProjectKind::WebFlex: return "webflex";
        case ProjectKind::GraphQL: return "graphql";
        case ProjectKind::Gateway: return "gateway";
        default: return "unknown";
    }
}
