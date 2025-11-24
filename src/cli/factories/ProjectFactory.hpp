/**
 * @file ProjectFactory.hpp
 * @brief Factory for creating project builders based on project type.
 * 
 * This class uses the Factory design pattern to return an appropriate
 * ProjectBuilder instance based on the provided ProjectKind. It supports
 * all project types defined in ProjectKind.
 * 
 * @author: Ko Thein (Nathan Mratt)
 * @date: Jan 2, 2022
*/

#ifndef LYNX_CLI_PROJECT_FACTORY_HPP
#define LYNX_CLI_PROJECT_FACTORY_HPP

#include <memory>
#include "interfaces/Builder.hpp"
#include "interfaces/ProjectKind.hpp"
#include "builders/WebAppBuilder.hpp"
#include "builders/ConsoleAppBuilder.hpp"
#include "builders/GenericProjectBuilder.hpp"

namespace LynxCLI {

    class ProjectFactory {

        public:

            /**
             * @brief Create a project builder for a given ProjectKind.
             * @param kind Type of project to create.
             * @return Unique pointer to a ProjectBuilder.
             * @throws std::invalid_argument if the project type is unsupported.
            */
            static std::unique_ptr<Builder> createBuilder(ProjectKind kind) {
                switch (kind) {
                    case ProjectKind::Web:
                        return std::make_unique<WebAppBuilder>();
                    case ProjectKind::Console:
                        return std::make_unique<ConsoleAppBuilder>();
                    case ProjectKind::Generic:
                    case ProjectKind::Library:
                    case ProjectKind::Service:
                    case ProjectKind::Mobile:
                    case ProjectKind::Game:
                    case ProjectKind::Plugin:
                    case ProjectKind::Test:
                    case ProjectKind::Microservice:
                    case ProjectKind::DataPipeline:
                    case ProjectKind::REST:
                    case ProjectKind::WebFlex:
                    case ProjectKind::GraphQL:
                    case ProjectKind::Gateway:
                    default:
                        return std::make_unique<GenericProjectBuilder>();  
                }    
            }

    };

}

#endif 
