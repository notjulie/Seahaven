
#include "json.hpp"

namespace json {

   JSON JSON::Load(const string& str) {
      size_t offset = 0;
      return std::move(parse_next(str, offset));
   }

   std::ostream& operator<<(std::ostream& os, const JSON& json) {
      os << json.dump();
      return os;
   }

   JSON Array() {
      return std::move(JSON::Make(JSON::Class::Array));
   }

   JSON Object() {
      return std::move(JSON::Make(JSON::Class::Object));
   }

}
