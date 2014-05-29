function show_dot_graph(id, format) {
  var result;
  try {
    result = Viz(src(id), format);
    if (format === "svg")
      return result;
    else
      return inspect(result);
  } catch(e) {
    return inspect(e.toString());
  }
}
function src(id) {
  return document.getElementById(id).innerHTML;
}  
function inspect(s) {
  return "<pre>" + s.replace(/</g, "&lt;").replace(/>/g, "&gt;").replace(/\"/g, "&quot;") + "</pre>"
}    