

// don't change those includes
#include "User.h"
#include "RecommendationSystem.h"


// implement your cpp code here
User::User (std::string name, rank_map rankings, const std::shared_ptr<RecommendationSystem> &rs)
    : _name (std::move(name)), _rankings (std::move(rankings)), _rs_ptr (rs)
{}

std::string User::get_name () const
{
  return _name;
}

rank_map User::get_ranks () const
{
  return _rankings;
}


void
User::add_movie_to_rs (const std::string &name, int year, const std::vector<double> &features, double rate)
{
  // Add the movie to RecommendationSystem DB
   sp_movie movie =  _rs_ptr->add_movie (name , year , features);

   // Add the movie to User DB
  _rankings.insert ({movie , rate});

}

//
double
User::get_prediction_score_for_movie (const std::string &name, int year, int k) const
{
  // save the movie
  sp_movie movie = _rs_ptr->get_movie (name,year);

  // score based on algorithm
  double num = _rs_ptr->predict_movie_score (*this ,movie , k);
  return num;
}


// by cf the movie
sp_movie User::get_recommendation_by_cf (int k) const
{
  // find recommend
  sp_movie movie= _rs_ptr->recommend_by_cf (*this , k);
  return movie;
}


// by the tochen
sp_movie User::get_recommendation_by_content () const
{
  // find recommend
  sp_movie movie= _rs_ptr->recommend_by_content(*this );
  return movie ;
}


std::ostream & operator<<(std::ostream& os , const User &man)
{
  // get the DB of RecommendationSystem
  rank_map temp = man.get_ranks();

  // print the users name
  os << "name:" << " <" << man.get_name() << ">\n";

  // print all movies
  for (auto &movie_pair : temp)
  {
    os << *movie_pair.first <<" " ;
  }
  os<< std::endl;
  return os;

}