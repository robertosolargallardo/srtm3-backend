#include <memory>
#include <cstdlib>
#include <restbed>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "../include/Logger.h"
#include "../include/SrtmHgtReader.h"
#define HEADERS multimap<string,string>{{"Connection","close"},\
                                        {"Content-Type","application/json"},\
                                        {"Access-Control-Allow-Methods","GET,PUT,POST,DELETE,OPTIONS"},\
                                        {"Access-Control-Allow-Origin","*"},\
                                        {"Access-Control-Allow-Headers","Content-Type, Authorization, Content-Length, X-Requested-With, Origin, Accept"}}

using namespace std;
using namespace restbed;

shared_ptr<SrtmHgtReader> srtm;

boost::property_tree::ptree run(double _lat,double _lon){
	boost::property_tree::ptree fresponse;
	fresponse.put("elevation",srtm->getElevation(_lat,_lon));
	return(fresponse);
}

void get_method(const shared_ptr<Session> &_session){
   	_session->yield(OK,HEADERS,[](const shared_ptr<Session> _session){});

		const auto& request=_session->get_request();
		string params=request->get_query_parameters().begin()->first;
		vector<string> gis;
		boost::split(gis,params,boost::is_any_of(","));
		
		ostringstream os;
      boost::property_tree::ptree fresponse=run(boost::lexical_cast<double>(gis[0]),boost::lexical_cast<double>(gis[1]));
      write_json(os,fresponse);

      _session->close(os.str());
}
void options(const shared_ptr<Session> &_session){
   	_session->close(OK,HEADERS);
}
int main(const int argc, const char **argv)
{
	if(argc<2){
		cerr << "Error::not enough arguments" << endl;
		cerr << "./srtm <path-srtm3-database>" << endl;
		exit(EXIT_FAILURE);
	}
	srtm=make_shared<SrtmHgtReader>(argv[1]);

	auto resource=make_shared<Resource>();
   resource->set_path("elevation");
   resource->set_method_handler("OPTIONS",options);
   resource->set_method_handler("GET",get_method);

   auto settings=make_shared<Settings>();
   settings->set_port(5001);
   settings->set_default_header("Connection","close");

   Service service;
   service.publish(resource);
	service.set_logger(make_shared<CustomLogger>());
   service.start(settings);

   return(EXIT_SUCCESS);
}
