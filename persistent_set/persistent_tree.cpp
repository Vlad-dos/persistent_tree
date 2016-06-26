#include "persistent_tree.h"

persistent_tree::persistent_tree() : root() {
	root.push_back(nullptr);
}

persistent_tree::persistent_tree(persistent_tree const& other) : root(other.root) {}

persistent_tree& persistent_tree::operator=(persistent_tree const& other) {
	this->root = other.root;

	return *this;
}

bool persistent_tree::empty(size_t version) {
	return root[version] == nullptr;
}

void persistent_tree::insert(size_t version, std::string value) {
	if (empty(version)) {
		root.push_back(std::make_shared<const node>(node(nullptr, nullptr, value)));
	} else {
		root.push_back(root[version]->insert(value));
	}
}

void persistent_tree::erase(size_t version, std::string value) {
	if (empty(version)) {
		root.push_back(nullptr);
	} else {
		root.push_back(root[version]->erase(value));
	}
}

bool persistent_tree::contains(size_t version, std::string value) {
	if (empty(version)) {
		return false;
	} else {
		return root[version]->contains(value);
	}
}

persistent_tree::node::node(std::shared_ptr<const node> left, std::shared_ptr<const node> right, std::string value)
	: left(left),
	right(right),
	value(std::move(value)) {}

std::string persistent_tree::node::min() const {
	if (left == nullptr)
		return value;
	return left->min();
}

std::string persistent_tree::node::max() const {
	if (right == nullptr)
		return value;
	return right->max();
}

persistent_tree::shared_node persistent_tree::node::insert(const std::string value) const {
	if (value == this->value) {
		return std::make_shared<const node>(*this);
	}
	shared_node left;
	shared_node right;
	if (value > this->value) {
		left = this->left;
		if (this->right != nullptr) {
			right = this->right->insert(value);
		} else {
			right = std::make_shared<const node>(node(nullptr, nullptr, value));
		}
	} else {
		right = this->right;
		if (this->left != nullptr) {
			left = this->left->insert(value);
		} else {
			left = std::make_shared<const node>(node(nullptr, nullptr, value));
		}
	}
	return std::make_shared<const node>(node(left, right, this->value));
}

persistent_tree::shared_node persistent_tree::node::erase(const std::string value) const {
	if (value == this->value) {
		if (this->left == nullptr) {
			return this->right;
		} else if (this->right == nullptr) {
			return this->left;
		} else {
			std::string new_value = right->min();
			return std::make_shared<const node>(node(left, right->erase(new_value), new_value));
		}
	}
	shared_node left;
	shared_node right;
	if (value > this->value) {
		left = this->left;
		if (this->right != nullptr) {
			right = this->right->erase(value);
		} else {
			right = nullptr;
		}
		return std::make_shared<const node>(node(left, right, this->value));
	} else {
		right = this->right;
		if (this->left != nullptr) {
			left = this->left->erase(value);
		} else {
			left = nullptr;
		}
	}
	return std::make_shared<const node>(node(left, right, this->value));
}

bool persistent_tree::node::contains(const std::string value) const {
	if (this->value == value) {
		return true;
	}

	if (this->value > value) {
		if (left == nullptr)
			return false;
		return left->contains(value);
	} else {
		if (right == nullptr)
			return false;
		return right->contains(value);
	}
}
