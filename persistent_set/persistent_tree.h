#ifndef PERSISTENT_TREE_H
#define PERSISTENT_TREE_H

#include <memory>
#include <string>
#include <vector>

struct persistent_tree {
	persistent_tree();
	persistent_tree(persistent_tree const& other);
	persistent_tree& operator=(persistent_tree const& other);

	bool empty(size_t version);
	void insert(size_t version, std::string value);
	void erase(size_t version, std::string value);
	bool contains(size_t version, std::string value);

	inline size_t get_version() const {
		return root.size() - 1;
	}
private:
	struct node {
		std::shared_ptr<const node> left, right;
		const std::string value;

		node(std::shared_ptr<const node> left, std::shared_ptr<const node> right, std::string value);
	
		std::string min() const;
		std::string max() const;

		std::shared_ptr<const node> insert(const std::string value) const;
		std::shared_ptr<const node> erase(const std::string value) const;
		bool contains(const std::string value) const;
	};

	typedef std::shared_ptr<const persistent_tree::node> shared_node;
	
	std::vector<shared_node> root;
};

#endif // PERSISTENT_TREE_H