
// src/infrastructure/GeminiParser.h

#include "../core/DataModels.h"
#include <string>

namespace tutor::infrastructure {

    using tutor::core::Curriculum;

    std::string ExtractJsonContent(const std::string& apiResponseText);

    Curriculum TransformJsonToCurriculum(const std::string& json_string,
                                         const std::string& language,
                                         const std::string& standard);
}

