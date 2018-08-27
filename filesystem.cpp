#include<iostream>
#include<filesystem>
#include <boost/filesystem.hpp>

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

void test_std_filesystem()
{
	std::cout << "Standard Filesystem Test Start" << std::endl;
	std::vector<std::filesystem::path> paths;
	add_test_paths(paths);
	print_test_paths(paths);
	compare_test_paths(paths);
	std::cout << "Standard Filesystem Test End" << std::endl;
}

void test_boost_filesystem()
{
	std::cout << "Boost Filesystem Test Start" << std::endl;
	std::vector<boost::filesystem::path> paths;
	add_test_paths(paths);
	print_test_paths(paths);
	compare_test_paths(paths);
	std::cout << "Boost Filesystem Test End" << std::endl;
}

void patch_test_paths(std::vector<boost::filesystem::path> &paths)
{
	for(size_t i = 0; i < paths.size() ; i++){
		if(paths[i].generic_string().compare(0, 2, "//") == 0 && paths[i].generic_string().compare(0, 3, "///") != 0)
			paths[i] = boost::filesystem::path(paths[i].generic_string().substr(1));
	}
}

void test_patched_boost_filesystem()
{
	std::cout << "Patched Boost Filesystem Test Start" << std::endl;
	std::vector<boost::filesystem::path> paths;
	add_test_paths(paths);
	patch_test_paths(paths);
	print_test_paths(paths);
	compare_test_paths(paths);
	std::cout << "Patched Boost Filesystem Test End" << std::endl;
}

int main()
{
	test_std_filesystem();
	test_boost_filesystem();
	test_patched_boost_filesystem();
	return 0;
}
