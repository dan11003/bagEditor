/*##############################################################################################################################
### BY DANIEL ADOLFSSON
### USAGE: head to the folder containing target rosbag   "rosrun ndt_offline tf_writer --bag-file-path 2012-01-08_vel_odom.bag --data-set michigan --odom-topic /robot_odom" make sure the odometry message topic is specified by the command --odom-topic
<<<<<<< HEAD
###
*/
#include <ros/ros.h>
#include <rosbag/bag.h>
#include <rosbag/view.h>
#include <boost/foreach.hpp>
#include <boost/program_options.hpp>
#include <dirent.h>
#include <ros/package.h>
using namespace std;
namespace po = boost::program_options;

class bag_merger{
public:
  bag_merger(const string &filename1,const std::string &filename2, const std::string &directory_name, const std::string &output_name){

    std::vector<std::string> files;
    if( (filename1=="" || filename2=="") && directory_name!=""){
      cout<<"Opening folder folder:"<<directory_name<<endl;
      LocateRosBagFilePaths(directory_name,files);
    }
    else if(filename1!="" && filename2!=""){
      cout<<"Merge files:"<<filename1<<" & "<<filename2<<endl;
      files.push_back(filename1);
      files.push_back(filename2);
    }
    else{
      cout<<"No files to merge"<<endl;
      exit(0);
    }

    bag_output_.open(output_name, rosbag::bagmode::Write);
    ConvertAll(files);
  }
  void ConvertAll(const std::vector<std::string> &files){
    int messages_converted=0;
    for(int i=0;i<files.size();i++){
      bag_in_.open(files[i]);
      view_ = new rosbag::View(bag_in_);
      I = view_->begin();
      while (I!=view_->end()) {
        rosbag::MessageInstance const m = *I++;
        bag_output_.write(m.getTopic(),m.getTime(),m);
      }
      bag_in_.close();
    }
    bag_output_.close();
  }
  bool LocateRosBagFilePaths(const std::string &folder_name,std::vector<std::string> &scanfiles){
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir (folder_name.c_str())) != NULL) {
      while ((ent = readdir (dir)) != NULL) {
        if(ent->d_name[0] == '.') continue;
        char tmpcname[400];
        snprintf(tmpcname,399,"%s/%s",folder_name.c_str(),ent->d_name);
        std::string tmpfname = tmpcname;
        scanfiles.push_back(tmpfname);
      }
      closedir (dir);
    } else {
      std::cerr<<"Could not parse dir name\n";
      return false;
    }
    sort(scanfiles.begin(),scanfiles.end());
    {
      std::cout << "files to be loaded : " << std::endl;
      for (size_t i = 0; i < scanfiles.size(); i++) {
        std::cout << " " << scanfiles[i] << std::flush;
      }
      std::cout << std::endl;
    }
    return true;
  }

private:
  rosbag::Bag bag_in_,bag_output_;
  rosbag::View *view_;
  rosbag::View::iterator I;

};

int main(int argc, char **argv){

  ros::Time::init();
  string inbag_name1,inbag_name2,directory_name,output_filename;
  po::options_description desc("Allowed options");
  desc.add_options()
      ("help", "produce help message")
      ("bag-file1", po::value<string>(&inbag_name1)->default_value(""), "bag file 1 to be merged")
      ("bag-file2", po::value<string>(&inbag_name2)->default_value(""), "bag file 2 to be merged")
      ("directory", po::value<string>(&directory_name)->default_value(""), "direcotry to bags")
      ("output-name", po::value<string>(&output_filename)->default_value("merged.bag"), "name of output")
      ;

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help")) {
    cout << desc << "\n";
    return 1;
  }
  bag_merger merge_bag(inbag_name1,inbag_name2,directory_name,output_filename);

  return 0;
}
