#ifndef LYNX_CLI_BUILDER_HPP
#define LYNX_CLI_BUILDER_HPP

/**
 * @file Builder.hpp
 * @brief Abstract base class for project builders.
 * 
 * Defines the interface for building different types of projects.
 * Concrete builders like WebAppBuilder or ConsoleAppBuilder will
 * implement the specific project structure generation logic.
 * 
 * Author: Ko Thein (Nathan Mratt)
 * Date: November 2, 2024
*/

#include <string>
#include "ProjectKind.hpp"

namespace LynxCLI {

    class Builder {

        protected:

            std::string projectName;

        public:

            /**
             * @brief Set the project name.
             * 
             * @param name The name of the project.
             * 
             * This sets the internal `projectName` variable used
             * by the builder during the build process.
            */
            virtual void setProjectName(const std::string& name) { projectName = name; }

            /**
             * @brief Generate project files and folders.
             * 
             * Concrete builders must implement this method to create the 
             * required folder structure, configuration files, and initial 
             * boilerplate code for the specific project type.
             */
            virtual void build() = 0;

            /**
             * @brief Default virtual destructor.
             * 
             * Ensures proper cleanup of derived classes.
            */
            virtual ~Builder() noexcept = default;    

    };

}


#endif
