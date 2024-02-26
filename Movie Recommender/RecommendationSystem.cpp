#include "RecommendationSystem.h"
#include <numeric>
#include <limits>
#include <cmath>
#include "algorithm"

// type of movie and rank of user for movie
typedef std::pair<sp_movie, double> pair_rank;

namespace normalize
{
    // using in name space
    using std::vector;
    using std::pair;

    double sum_pairs (double acc, const pair_rank &pair)
    {
      return acc + pair.second;
    }

    double get_avg (const vector<pair_rank> &rank)
    {
      // sum all the ranking of the movies
      // sum_pairs return the rank from each pair
      double sum = std::accumulate (rank.begin (), rank.end (), 0.0, sum_pairs);

      // div with the amount of movies
      double avg = sum / rank.size ();

      // return avg
      return avg;
    }

    vector<pair_rank> create_vector (const rank_map
                                     &user_ranks)
    {
      // start new vec , there we will save pairs
      vector<pair_rank> new_vec;

      // loop over rank map
      for (const auto &movie_pair: user_ranks)
      {
        // declare a pair of <movie , rank>
        pair_rank temp = movie_pair;

        // add to vector
        new_vec.push_back (temp);
      }
      return new_vec;
    }

    vector<pair_rank> create_not_seen_vector (const recommendation_ranks
                                              &rs_rank, const rank_map
                                              &user_ranks)
    {
      // start new vec , there we will save pairs
      vector<pair_rank> new_vec;

      // loop over rank map
      for (const auto &movie_pair: user_ranks)
      {
        // declare a pair of <movie , rank>
        pair_rank temp = movie_pair;

        // add to vector
        new_vec.push_back (temp);
      }
      for (const auto &movie_pair: rs_rank)
      {
        // Check if the target elem
        sp_movie movie = movie_pair.first;

        // check if in vector
        const auto iter = std::find_if (new_vec.begin (), new_vec.end (),
                                        [&movie]
                                            (const pair_rank &p)
                                        { return sp_movie_equal (p.first, movie); });

        // not in vector
        if (iter == new_vec.end ())
        {
          new_vec.emplace_back (movie_pair.first, std::numeric_limits<double>::lowest ());
        }
        else
        {
          new_vec.erase (iter);
        }

      }
      return new_vec;

    }

    void subtract_average (vector<pair_rank> &rank, double avg)
    {
      // loop over the vector
      for (auto &movie_pair: rank)
      {
        // subtract the average
        movie_pair.second -= avg;
      }
    }

    vector<pair_rank>
    normalize_vec (const rank_map &ranks)
    {

      // create a vector of pair of  movies user saw <movie , rank > for the
      // user
      vector<pair_rank> seen_pair_vec = create_vector (ranks);

      // get avg
      double avg = get_avg (seen_pair_vec);

      //subtract the average
      subtract_average (seen_pair_vec, avg);
      return seen_pair_vec;
    }

}

namespace preferred
{

    using std::vector;

    double vector_norm (const std::vector<double> &vec)
    {
      double sum = 0.0;

      // sum all the
      for (const double &element: vec)
      {
        sum += element * element;
      }

      double norm = std::sqrt (sum);
      return norm;
    }

    double vector_dot (const std::vector<double> &vec1, const
    std::vector<double> &vec2)
    {

      // declare new vector
      size_t size = std::min (vec1.size (), vec2.size ());
      double sum = 0.0;
      for (size_t i = 0; i < size; ++i)
      {
        sum += (vec1[i] * vec2[i]);
      }

      return sum;
    }

    void
    add_vector (std::vector<double> &vec1, const std::vector<double> &vec2)
    {
      size_t size = std::min (vec1.size (), vec2.size ());

      // loop over all elent in vector
      for (size_t i = 0; i < size; ++i)
      {
        // add to vector 1
        vec1[i] += vec2[i];
      }
    }

    vector<double> get_preferred (recommendation_ranks &rank,
                                  const vector<pair_rank> &pair_vec)
    {
      // get size of movie
      int size = (int) rank[pair_vec[0].first].size ();

      // start a vector in with size amount of elem to 0
      vector<double> ret_vec (size, 0.0);

      // all the user saw this movie and rated
      for (const auto &movie_pair: pair_vec)
      {
        double scalar = movie_pair.second;
        vector<double> temp = rank[movie_pair.first];

        // Multiply each element in the vector by the scalar
        std::transform (temp.begin (), temp.end (), temp.begin (), [scalar] (double value)
        { return value * scalar; });

        // add to ret_vec
        add_vector (ret_vec, temp);

      }
      return ret_vec;
    }

    double
    find_similarity (const vector<double> &contect_rank_vec, const vector<double> &pref_rank_vec)
    {
      // each element times each other
      double top = vector_dot (contect_rank_vec, pref_rank_vec);

      // multiply the norms
      double bottom =
          vector_norm (contect_rank_vec) * vector_norm (pref_rank_vec);

      return top / bottom;
    }

    bool pair_size (const pair_rank &p1, const pair_rank &p2)
    {
      return p1.second < p2.second;
    }

    /**
     * @param pair_vec
     *  the function will use this comparison function
     *  to determine the maximum element based on the second element of the pairs in the vector.
     * @return the pair with the maximum double
     */
    pair_rank find_max_second_element (const vector<pair_rank> &pair_vec)
    {
      // find maximum second pair using the pair_size function
      const auto max_iter = std::max_element (pair_vec.begin (), pair_vec.end
          (), pair_size);
      return *max_iter;

    }
    // check to change to a vec of not seen vectors
    sp_movie
    check_similarity (recommendation_ranks &rank, vector<pair_rank> &not_seen_movie_vec, vector<double> &pref_rank_vec)
    {
      for (auto &movie_pair: not_seen_movie_vec)
      {

        // for each movie that the user didnt rate - didnt watch changing its
        // rate to the similarity
        movie_pair.second = find_similarity (rank[movie_pair.first], pref_rank_vec);
      }
      pair_rank ret = find_max_second_element (not_seen_movie_vec);

      // return movie
      return ret.first;

    }

}

namespace
{
    using std::vector;

    bool pair_size (const pair_rank &p1, const pair_rank &p2)
    {
      return p1.second > p2.second;
    }

    double
    calculate_k_top_sum_rating (const vector<pair_rank> &k_top_scores, const
    vector<pair_rank> &seen_vec)
    {
      double top = 0;
      double buttom = 0;
      // the top not seen movies
      for (const auto &movie_pair: k_top_scores)
      {
        for (const auto &seen_pair: seen_vec)
        {
          if (sp_movie_equal (movie_pair.first, seen_pair.first))
          {
            // times the Similarity with the user ranking and sum
            top += seen_pair.second * movie_pair.second;

            // sum all the Similarity or the user ranking
            buttom += movie_pair.second;
          }

        }
      }
      return top / buttom;
    }

    double
    comp_to_seen_movies (recommendation_ranks &rank, vector<double>
    &not_seen_movie_vec, vector<pair_rank> &seen_vec, int k)
    {
      vector<pair_rank> k_top_scores = seen_vec;
      // loop throw all the seen vectors
      for (auto &movie_pair: k_top_scores)
      {

        movie_pair.second = preferred::find_similarity (not_seen_movie_vec, rank[movie_pair.first]);
      }

      std::sort (k_top_scores.begin (), k_top_scores.end (), pair_size);
      k_top_scores.erase (k_top_scores.begin () + k, k_top_scores.end ());

      return calculate_k_top_sum_rating (k_top_scores, seen_vec);
    }

    pair_rank
    find_cf_movie (recommendation_ranks &rank, const vector<pair_rank>
    &not_seen_vec, vector<pair_rank> &seen_vec, int k)
    {
      // the vector going to return
      vector<pair_rank> ret_vec;

      // a vector only with the movies the user saw

      // loop over the not seen movies by user in the DB
      for (const auto &movie_pair: not_seen_vec)
      {

        ret_vec.emplace_back (movie_pair.first, comp_to_seen_movies (rank, rank[movie_pair.first], seen_vec, k));
      }
      // return the movie with the highest score
      pair_rank result = preferred::find_max_second_element (ret_vec);
      return result;

    }

}

sp_movie RecommendationSystem::add_movie (const std::string &name, int year,
                                          const std::vector<double> &features)
{
  sp_movie movie = std::make_shared<Movie> (name, year);

  // add to _movie_ranks map
  _movie_ranks.insert ({movie, features});

  //_movie_vec.push_back (movie);
  movie_top_p.insert ({*movie, movie});
  // return shared pointer
  return movie;
}

sp_movie
RecommendationSystem::get_movie (const std::string &name, int year) const
{
  sp_movie temp = std::make_shared<Movie> (name, year);

  // save the Movie place in iter
  auto it = movie_top_p.find (*temp);

  // check if the it is last
  if (it != movie_top_p.end ())
  {
    // let go of memory , and set to nullptr
    return it->second;
  }
  return nullptr;
}

sp_movie RecommendationSystem::recommend_by_content (const User &user)
{
  // create a vector of pair <movie , rank > for the user and normalize
  std::vector<pair_rank> new_vec = normalize::normalize_vec (user.get_ranks ());

  // get preferred vector of content for user
  std::vector<double> pref_vec = preferred::get_preferred (_movie_ranks, new_vec);

  // create a vector of pair of all movies <movie , rank > for the user
  std::vector<pair_rank> not_seen_vector = normalize::create_not_seen_vector (_movie_ranks, user.get_ranks ());

  // find similarity movie
  sp_movie ret = preferred::check_similarity (_movie_ranks, not_seen_vector, pref_vec);
  return ret;
}

sp_movie RecommendationSystem::recommend_by_cf (const User &user, int k)
{
  // create a vector of pair <movie , rank > not seen movies by user
  std::vector<pair_rank> not_seen_vector = normalize::create_not_seen_vector (_movie_ranks, user.get_ranks ());

  // create a vector of pair <movie , rank > seen movies by user
  std::vector<pair_rank> movie_seen_vector = normalize::create_vector (user.get_ranks ());

  // find movie by movies that were watched
  pair_rank result = find_cf_movie (_movie_ranks, not_seen_vector,
                                    movie_seen_vector, k);
  return result.first;
}

double
RecommendationSystem::predict_movie_score (const User &user, const sp_movie &movie, int k)
{
  rank_map ranks = user.get_ranks ();

  // create a vector of pair <movie , rank > for the user
  std::vector<pair_rank> movie_seen_vector = normalize::create_vector (user.get_ranks ());

  // create a single vector af not watched
  std::vector<pair_rank> not_seen_vector;
  not_seen_vector.emplace_back (movie, 0);

  // find movie by movies that were watched
  pair_rank resulst = find_cf_movie (_movie_ranks, not_seen_vector, movie_seen_vector, k);
  return resulst.second;

}

std::ostream &operator<< (std::ostream &os, const RecommendationSystem &rec)
{
  for (const auto &movie: rec.movie_top_p)
  {
    std::cout << movie.first << " ";
  }
  return os;
}