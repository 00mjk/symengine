/**
 *  \file sets.h
 *
 **/
#ifndef SYMENGINE_SETS_H
#define SYMENGINE_SETS_H

#include <symengine/functions.h>
#include <symengine/complex.h>
#include <iterator>

namespace SymEngine
{

class Set : public Basic
{
public:
    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const = 0;
    virtual RCP<const Set> set_union(const RCP<const Set> &o) const = 0;
    virtual bool contains(const RCP<const Basic> &a) const = 0;
    virtual bool is_subset(const RCP<const Set> &o) const = 0;
    virtual bool is_proper_subset(const RCP<const Set> &o) const = 0;
    virtual bool is_superset(const RCP<const Set> &o) const = 0;
    virtual bool is_proper_superset(const RCP<const Set> &o) const = 0;
};

class EmptySet : public Set
{
private:
    EmptySet(){};

public:
    IMPLEMENT_TYPEID(EMPTYSET)
    // EmptySet(EmptySet const&) = delete;
    void operator=(EmptySet const &) = delete;
    const static RCP<const EmptySet> &getInstance();
    virtual std::size_t __hash__() const;
    virtual bool __eq__(const Basic &o) const;
    virtual int compare(const Basic &o) const;
    virtual vec_basic get_args() const
    {
        return {};
    }

    template <typename T_, typename... Args>
    friend inline RCP<T_> make_rcp(Args &&... args);

    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const;
    virtual RCP<const Set> set_union(const RCP<const Set> &o) const;
    virtual bool contains(const RCP<const Basic> &a) const
    {
        return false;
    };
    virtual bool is_subset(const RCP<const Set> &o) const
    {
        return true;
    };
    virtual bool is_proper_subset(const RCP<const Set> &o) const;
    virtual bool is_superset(const RCP<const Set> &o) const;
    virtual bool is_proper_superset(const RCP<const Set> &o) const
    {
        return false;
    };
};

class UniversalSet : public Set
{
private:
    UniversalSet(){};

public:
    IMPLEMENT_TYPEID(UNIVERSALSET)
    // UniversalSet(UniversalSet const&) = delete;
    void operator=(UniversalSet const &) = delete;
    const static RCP<const UniversalSet> &getInstance();
    virtual std::size_t __hash__() const;
    virtual bool __eq__(const Basic &o) const;
    virtual int compare(const Basic &o) const;
    virtual vec_basic get_args() const
    {
        return {};
    }

    template <typename T_, typename... Args>
    friend inline RCP<T_> make_rcp(Args &&... args);

    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const;
    virtual RCP<const Set> set_union(const RCP<const Set> &o) const;
    virtual bool contains(const RCP<const Basic> &a) const
    {
        return true;
    };
    virtual bool is_subset(const RCP<const Set> &o) const;
    virtual bool is_proper_subset(const RCP<const Set> &o) const
    {
        return false;
    };
    virtual bool is_superset(const RCP<const Set> &o) const
    {
        return true;
    };
    virtual bool is_proper_superset(const RCP<const Set> &o) const;
};

class FiniteSet : public Set
{
public:
    set_basic container_;

public:
    IMPLEMENT_TYPEID(FINITESET)
    virtual std::size_t __hash__() const;
    virtual bool __eq__(const Basic &o) const;
    virtual int compare(const Basic &o) const;
    virtual vec_basic get_args() const
    {
        return {};
    }

    FiniteSet(const set_basic container);
    static bool is_canonical(const set_basic container);

    virtual RCP<const Set> set_union(const RCP<const Set> &o) const;
    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const;
    virtual bool contains(const RCP<const Basic> &a) const;
    virtual bool is_subset(const RCP<const Set> &o) const;
    virtual bool is_proper_subset(const RCP<const Set> &o) const;
    virtual bool is_superset(const RCP<const Set> &o) const;
    virtual bool is_proper_superset(const RCP<const Set> &o) const;
};

class Interval : public Set
{
public:
    RCP<const Number> start_;
    RCP<const Number> end_;
    bool left_open_, right_open_;

public:
    IMPLEMENT_TYPEID(INTERVAL)
    virtual std::size_t __hash__() const;
    virtual bool __eq__(const Basic &o) const;
    virtual int compare(const Basic &o) const;

    Interval(const RCP<const Number> &start, const RCP<const Number> &end,
             const bool left_open = false, const bool right_open = false);

    RCP<const Set> open() const;
    RCP<const Set> close() const;
    RCP<const Set> Lopen() const;
    RCP<const Set> Ropen() const;

    static bool is_canonical(const RCP<const Number> &start,
                             const RCP<const Number> &end, bool left_open,
                             bool right_open);

    virtual RCP<const Set> set_union(const RCP<const Set> &o) const;
    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const;
    virtual bool contains(const RCP<const Basic> &a) const;
    virtual bool is_subset(const RCP<const Set> &o) const;
    virtual bool is_proper_subset(const RCP<const Set> &o) const;
    virtual bool is_superset(const RCP<const Set> &o) const;
    virtual bool is_proper_superset(const RCP<const Set> &o) const;
    virtual vec_basic get_args() const;
};

class Union : public Set
{
public:
    set_basic container_;

public:
    IMPLEMENT_TYPEID(UNION)
    virtual std::size_t __hash__() const;
    virtual bool __eq__(const Basic &o) const;
    virtual int compare(const Basic &o) const;
    virtual vec_basic get_args() const
    {
        return {};
    }
    Union(set_basic in);

    virtual RCP<const Set> set_intersection(const RCP<const Set> &o) const;
    virtual RCP<const Set> set_union(const RCP<const Set> &o) const;
    virtual bool contains(const RCP<const Basic> &a) const;
    virtual bool is_subset(const RCP<const Set> &o) const;
    virtual bool is_proper_subset(const RCP<const Set> &o) const;
    virtual bool is_superset(const RCP<const Set> &o) const;
    virtual bool is_proper_superset(const RCP<const Set> &o) const;
};

//! \return RCP<const EmptySet>
inline RCP<const EmptySet> emptyset()
{
    return EmptySet::getInstance();
}

//! \return RCP<const UniversalSet>
inline RCP<const UniversalSet> universalset()
{
    return UniversalSet::getInstance();
}

//! \return RCP<const Set>
inline RCP<const Set> finiteset(const set_basic &container)
{
    if (FiniteSet::is_canonical(container)) {
        return make_rcp<const FiniteSet>(container);
    }
    return emptyset();
}

//! \return RCP<const Set>
inline RCP<const Set> interval(const RCP<const Number> &start,
                               const RCP<const Number> &end,
                               const bool left_open = false,
                               const bool right_open = false)
{
    if (Interval::is_canonical(start, end, left_open, right_open))
        return make_rcp<const Interval>(start, end, left_open, right_open);
    if (eq(*start, *end) and not(left_open or right_open))
        return finiteset({start});
    return emptyset();
}

// ! \return RCP<const Set>
inline RCP<const Set> sym_union(const set_basic &in, bool solve = true)
{
    set_basic input;
    if (solve == false && in.size() > 1)
        return make_rcp<const Union>(in);
    set_basic combined_FiniteSet;
    for (auto it = in.begin(); it != in.end(); ++it) {
        if (is_a<FiniteSet>(**it)) {
            set_basic container;
            const FiniteSet &other = static_cast<const FiniteSet &>(**it);
            std::set_union(combined_FiniteSet.begin(), combined_FiniteSet.end(),
                           other.container_.begin(), other.container_.end(),
                           std::inserter(container, container.begin()),
                           RCPBasicKeyLess{});
            combined_FiniteSet = container;
        } else if (is_a<UniversalSet>(**it)) {
            return universalset();
        } else if (not is_a<EmptySet>(**it)) {
            input.insert(*it);
        }
    }
    if (input.empty()) {
        return finiteset(combined_FiniteSet);
    } else if (input.size() == 1 && combined_FiniteSet.empty()) {
        return rcp_dynamic_cast<const Set>(*input.begin());
    }
    // Now we rely on respective containers' own rules
    // TODO: Improve it to O(log n)
    RCP<const Set> combined_Rest = finiteset(combined_FiniteSet);
    for (auto it = input.begin(); it != input.end(); ++it) {
        RCP<const Set> other = rcp_dynamic_cast<const Set>(*it);
        combined_Rest = combined_Rest->set_union(other);
    }
    return combined_Rest;
}
}
#endif
