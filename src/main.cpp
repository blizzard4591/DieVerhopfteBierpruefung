#include <array>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <list>
#include <set>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

template<std::size_t ELEMENT_COUNT>
class Permutation {
public:
	Permutation() : m_data({ 0 }) {
		//
	}
	Permutation(std::array<std::size_t, ELEMENT_COUNT> const& data) : m_data(data) {
		//
	}

	std::array<std::size_t, ELEMENT_COUNT>& data() {
		return m_data;
	}

	std::size_t at(std::size_t i) const {
		return m_data[i];
	}

	bool isValidPermutation() const {
		// Layout: 
		// First Triangle: 0, 1, 2, 3 <-> 3, 4, 5, 6 <-> 6, 7, 8, 0
		// Second Triangle: 9, 2, 4, 10 <-> 10, 5, 7, 11 <-> 11, 8, 1, 9
		std::size_t const sum = 26;
		if ((m_data[0] + m_data[1] + m_data[2] + m_data[3]) != sum) return false;
		if ((m_data[3] + m_data[4] + m_data[5] + m_data[6]) != sum) return false;
		if ((m_data[6] + m_data[7] + m_data[8] + m_data[0]) != sum) return false;
		if ((m_data[9] + m_data[2] + m_data[4] + m_data[10]) != sum) return false;
		if ((m_data[10] + m_data[5] + m_data[7] + m_data[11]) != sum) return false;
		if ((m_data[11] + m_data[8] + m_data[1] + m_data[9]) != sum) return false;
		return true;
	}

	std::string toString() const {
		std::string result;
		result.reserve(128);
		for (std::size_t i = 0; i < ELEMENT_COUNT; ++i) {
			if (i > 0) {
				result.append(", ");
			}
			result.append(std::to_string(m_data[i]));
		}
		return result;
	}

	void rotateStar() {
		auto const temp = m_data;
		m_data[0] = temp[9];
		m_data[1] = temp[2];
		m_data[2] = temp[4];
		m_data[3] = temp[10];
		m_data[4] = temp[5];
		m_data[5] = temp[7];
		m_data[6] = temp[11];
		m_data[7] = temp[8];
		m_data[8] = temp[1];
		m_data[9] = temp[3];
		m_data[10] = temp[6];
		m_data[11] = temp[0];
	}

	void mirrorOnLongAxis() {
		std::swap(m_data[1], m_data[8]);
		std::swap(m_data[2], m_data[7]);
		std::swap(m_data[3], m_data[6]);
		std::swap(m_data[4], m_data[5]);
		std::swap(m_data[9], m_data[11]);
	}

	void mirrorOnShortAxis() {
		std::swap(m_data[0], m_data[11]);
		std::swap(m_data[1], m_data[7]);
		std::swap(m_data[2], m_data[5]);
		std::swap(m_data[3], m_data[10]);
		std::swap(m_data[6], m_data[9]);
	}

	friend std::ostream& operator<< (std::ostream& stream, Permutation const& p) {
		for (std::size_t i = 0; i < ELEMENT_COUNT; ++i) {
			if (i > 0) {
				stream << ", ";
			}
			stream << p.at(i);
		}
		return stream;
	}
private:
	std::array<std::size_t, ELEMENT_COUNT> m_data;
};

template<std::size_t ELEMENT_COUNT>
class Permutations {
public:
	Permutations(std::array<std::size_t, ELEMENT_COUNT> const& elements) : m_elements(elements), m_counters({0}) {
		for (std::size_t i = 0; i < ELEMENT_COUNT; ++i) {
			m_counters[i] = i;
		}
	}

	std::size_t getNextBiggerIndex(std::size_t start, bool& foundSomething) {
		std::size_t const compareElement = m_counters[start];
		std::size_t currentResult = ELEMENT_COUNT + 1;
		foundSomething = false;
		for (std::size_t i = start + 1; i < ELEMENT_COUNT; ++i) {
			std::size_t const element = m_counters[i];
			if (element > compareElement && element < currentResult) {
				currentResult = element;
				foundSomething = true;
			}
		}
		return currentResult;
	}

	void removeElementFromList(std::list<std::size_t>& container, std::size_t element) {
		auto it = std::find(container.begin(), container.end(), element);
		if (it != container.end()) {
			container.erase(it);
		}
	}

	bool getNextPermutation(Permutation<ELEMENT_COUNT>& p) {
		std::array<std::size_t, ELEMENT_COUNT>& target = p.data();
		for (std::size_t i = 0; i < ELEMENT_COUNT; ++i) {
			target[i] = m_elements[m_counters[i]];
		}

		// Build next permutation
		std::list<std::size_t> freeIndices;
		bool ok = false;

		// last two elements always go back into the pool.
		freeIndices.push_back(m_counters[ELEMENT_COUNT - 1]);

		std::size_t currentIndex = m_counters[ELEMENT_COUNT - 1];
		std::size_t lastIndex = -1;
		std::size_t maxIndex = currentIndex;
		for (int i = ELEMENT_COUNT - 2; i >= 0; --i) {
			lastIndex = currentIndex;
			currentIndex = m_counters[i];
			freeIndices.push_back(currentIndex);
			if (currentIndex < maxIndex) {
				// We can iterate here!
				std::size_t const nextElement = getNextBiggerIndex(i, ok);
				if (!ok) {
					std::cerr << "Failed to identify next item in line?!" << std::endl;
					return false;
				}
				m_counters[i] = nextElement;
				removeElementFromList(freeIndices, nextElement);
				freeIndices.sort();
				auto it = freeIndices.cbegin();
				for (std::size_t j = i + 1; j < ELEMENT_COUNT; ++j) {
					if (it == freeIndices.cend()) {
						std::cerr << "FreeIndices count missmatch?!" << std::endl;
						return false;
					}
					m_counters[j] = *it;
					++it;
				}
				return true;
			} else {
				// all done in this depth, need to go one up
				maxIndex = std::max(maxIndex, currentIndex);
			}
		}
		return false;
	}
private:
	std::array<std::size_t, ELEMENT_COUNT> const m_elements;
	std::array<std::size_t, ELEMENT_COUNT> m_counters;
};

template<std::size_t ELEMENT_COUNT>
std::vector<Permutation<ELEMENT_COUNT>> makeAllVariations(Permutation<ELEMENT_COUNT> const& p) {
	std::vector<Permutation<ELEMENT_COUNT>> result;
	result.reserve(3 * 6);
	
	Permutation<ELEMENT_COUNT> p2 = p;
	result.push_back(p2);
	for (int i = 0; i < 5; ++i) {
		p2.rotateStar();
		result.push_back(p2);
	}

	p2 = p;
	p2.mirrorOnLongAxis();
	result.push_back(p2);
	for (int i = 0; i < 5; ++i) {
		p2.rotateStar();
		result.push_back(p2);
	}

	p2 = p;
	p2.mirrorOnShortAxis();
	result.push_back(p2);
	for (int i = 0; i < 5; ++i) {
		p2.rotateStar();
		result.push_back(p2);
	}
	return result;
}

int main(int argc, char* argv[]) {
	std::cout << "Die verhopfte Bierprüfung." << std::endl;

	std::array<std::size_t, 12> stones = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
	Permutations<12> permutations(stones);

	std::set<std::string> knownSolutions;

	auto const beginTotal = std::chrono::steady_clock::now();
	Permutation<12> target;
	bool hasNext = true;

	std::size_t counterAll = 0;
	std::size_t counterCorrect = 0;
	std::size_t counterUniqueCorrect = 0;

	while (hasNext) {
		hasNext = permutations.getNextPermutation(target);
		
		++counterAll;
		if (target.isValidPermutation()) {
			++counterCorrect;
			bool isKnown = false;
			auto const variations = makeAllVariations(target);
			for (auto it = variations.cbegin(); it != variations.cend(); ++it) {
				auto const s = it->toString();
				if (knownSolutions.find(s) != knownSolutions.end()) {
					isKnown = true;
					break;
				}
			}
			if (!isKnown) {
				knownSolutions.insert(target.toString());
				++counterUniqueCorrect;
			}
		}
		
		if (counterAll % 25000000 == 0) {
			std::cout << "At " << counterAll << " with " << counterCorrect << " correct permutations and " << counterUniqueCorrect << " unique correct permutations." << std::endl;
			std::cout << "Current permutation: " << target << std::endl;
		}
	}
	auto const endTotal = std::chrono::steady_clock::now();

	std::cout << "Looked at " << counterAll << " with " << counterCorrect << " correct permutations and " << counterUniqueCorrect << " unique correct permutations in " << std::chrono::duration_cast<std::chrono::milliseconds>(endTotal - beginTotal).count() << "ms." << std::endl;

	std::cout << "All unique solutions:" << std::endl;
	for (auto it = knownSolutions.cbegin(); it != knownSolutions.cend(); ++it) {
		std::cout << *it << std::endl;
	}

	std::cout << "Done!" << std::endl;
	return 0;
}

#ifdef _MSC_VER
int __stdcall WinMain(struct HINSTANCE__* hInstance, struct HINSTANCE__* hPrevInstance, char* lpszCmdLine, int nCmdShow) {
	return main(__argc, __argv);
}

#endif
