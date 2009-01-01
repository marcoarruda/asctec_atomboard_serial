bool treatValidation(std_msgs::String s) {
  return s.data.length() > 8;
}

void treatConvert(std_msgs::String s, std_msgs::Int16MultiArray &array) {
  // ignore brackets
  std_msgs::String s1, sc;
  std::stringstream ssc;
  s1.data = s.data.substr(2, s.data.length() - 4);

  int pos = 0, value;
  array.data.clear();
  while(pos > -1) {
    pos = s1.data.find(';');
    sc.data = s1.data.substr(0, pos);
    s1.data = s1.data.substr(pos + 1, s1.data.length() - (pos + 1));
    ssc.str(sc.data);
    ssc >> value;
    ssc.clear();
    ssc.str("");
    //ROS_INFO("%d", value);
    array.data.push_back(value);
  }
}
