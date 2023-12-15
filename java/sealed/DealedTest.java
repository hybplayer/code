package sealed;

import java.util.ArrayList;
import java.util.HashMap;

sealed interface JSONValue permits JSONArray, JSONObject, JSONPrimitive {
public default String
  type() {
    if (this instanceof JSONArray)
      return "array";
    else if (this instanceof JSONObject)
      return "object";
    else if (this instanceof )
  }
}

final class JSONArray extends ArrayList<JSONValue> implements JSONValue {}

final class JSONObject extends HashMap<String, JSONValue> implements JSONValue {
  < public String toString() {
    StringBuilder result = new StringBuilder();
    result.append("{");
  }
}