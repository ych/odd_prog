#include <iostream>
#include <filesystem>
#include <boost/filesystem.hpp>
#include <boost/core/demangle.hpp>
#include <boost/mp11.hpp>

template<class T> std::string name()
{
    return boost::core::demangle( typeid(T).name() );
}

template <class T>
void patch_test_paths(std::vector<T> &paths)
{
	if(name<T>() != "boost::filesystem::path")
		return;
	for(size_t i = 0; i < paths.size() ; i++){
		if(paths[i].generic_string().compare(0, 2, "//") == 0 && paths[i].generic_string().compare(0, 3, "///") != 0)
			paths[i] = T(paths[i].generic_string().substr(1));
	}
}

template <class T>
void add_test_paths(std::vector<T> &paths)
{
	paths.push_back(T("/./"));
	paths.push_back(T("/../"));
	paths.push_back(T("//./"));
	paths.push_back(T("//../"));
	paths.push_back(T("///./"));
	paths.push_back(T("///../"));
	paths.push_back(T("////./"));
	paths.push_back(T("////../"));
	paths.push_back(T("/././"));
	paths.push_back(T("/../../"));
	paths.push_back(T("/..//..//"));
	paths.push_back(T("/..///..///"));
	paths.push_back(T("/"));
	paths.push_back(T("//"));
	paths.push_back(T("///"));
	paths.push_back(T("////"));
}

template <class T>
void print_test_paths(const std::vector<T> &paths)
{
	for(auto it: paths){
		std::cout << weakly_canonical(it) << std::endl;
	}
}

template <class T>
void compare_test_paths(const std::vector<T> &paths)
{
	for(size_t i = 0; i < paths.size() ; i++){
		for(size_t j = i+1; j < paths.size(); j++){
			if(weakly_canonical(paths[i]) != weakly_canonical(paths[j]))
				std::cout << paths[i] << "is diffent from " << paths[j] << std::endl;
		}
	}
}


template<class T> void test_filesystem_path( boost::mp11::mp_list<T> const& )
{
	std::cout << name<T>() << " Test Start" << std::endl;
	std::vector<T> paths;
	add_test_paths(paths);
	if(name<T>() == "boost::filesystem::path")
		patch_test_paths(paths);
	print_test_paths(paths);
	compare_test_paths(paths);
	std::cout << name<T>() << " Test End" << std::endl;
}

int main()
{
	using L = std::tuple<std::filesystem::path, boost::filesystem::path>;
	boost::mp11::tuple_for_each( boost::mp11::mp_product<boost::mp11::mp_list, L>(), [](auto&& x){ test_filesystem_path(x); } );
	return 0;
}
