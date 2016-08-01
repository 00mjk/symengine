#include <symengine/polynomial_multivariate.h>

namespace SymEngine
{

vec_basic MultivariateIntPolynomial::get_args() const
{
    vec_basic args;
    umap_uvec_mpz d;
    // To change the ordering in which the terms appear in the vector, use
    // a different comparator for order_umap
    auto v = sorted_keys(dict_);
    for (const auto &p : v) {
        map_basic_basic b;
        int whichvar = 0;
        for (auto sym : vars_) {
            if (integer_class(0) != p[whichvar])
                insert(b, sym, integer(p[whichvar]));
            whichvar++;
        }
        args.push_back(
            Mul::from_dict(integer(dict_.find(p)->second), std::move(b)));
    }
    return args;
}

std::size_t MultivariateIntPolynomial::__hash__() const
{
    std::size_t seed = MULTIVARIATEINTPOLYNOMIAL;
    for (auto var : vars_)
        hash_combine<std::string>(seed, var->__str__());

    for (auto &p : dict_) {
        std::size_t t = vec_uint_hash()(p.first);
        hash_combine<std::size_t>(t, mp_get_si(p.second));
        seed ^= t;
    }
    return seed;
}

integer_class MultivariateIntPolynomial::eval(
    std::map<RCP<const Basic>, integer_class, RCPBasicKeyLess> &vals) const
{
    integer_class ans(0);
    for (auto bucket : dict_) {
        integer_class term = bucket.second;
        unsigned int whichvar = 0;
        for (auto sym : vars_) {
            integer_class temp;
            mp_pow_ui(temp, vals.find(sym)->second, bucket.first[whichvar]);
            term *= temp;
            whichvar++;
        }
        ans += term;
    }
    return ans;
}

RCP<const MultivariateIntPolynomial>
MultivariateIntPolynomial::convert(const UIntPoly &o)
{
    vec_basic s;
    s.push_back(o.get_var());

    umap_uvec_mpz d;
    for (auto &p : o.get_dict()) {
        vec_uint v;
        v.push_back(p.first);
        d[v] = p.second;
    }
    return MultivariateIntPolynomial::create(s, std::move(d));
}

unsigned int reconcile(vec_uint &v1, vec_uint &v2, set_basic &s,
                       const set_basic &s1, const set_basic &s2)
{
    auto a1 = s1.begin();
    auto a2 = s2.begin();
    unsigned int poscount = 0;
    // Performs a merge sort of s1 and s2, and builds up v1 and v2 as
    // translators:
    // v[i] is the position of the ith symbol in the new set.
    while (a1 != s1.end() && a2 != s2.end()) {
        if ((*a1)->__eq__(**a2) && (a1 != s1.end() && a2 != s2.end())) {
            v1.insert(v1.end(), poscount);
            v2.insert(v2.end(), poscount);
            s.insert(*a1);
            a1++;
            a2++;
        } else if (RCPBasicKeyLess()(*a1, *a2)) {
            v1.insert(v1.end(), poscount);
            s.insert(*a1);
            a1++;
        } else {
            v2.insert(v2.end(), poscount);
            s.insert(*a2);
            a2++;
        }
        poscount++;
    }
    if (a1 == s1.end()) {
        while (a2 != s2.end()) {
            v2.insert(v2.end(), poscount);
            s.insert(*a2);
            a2++;
            poscount++;
        }
    } else if (a2 == s2.end()) {
        while (a1 != s1.end()) {
            v1.insert(v1.end(), poscount);
            s.insert(*a1);
            a1++;
            poscount++;
        }
    }
    return poscount; // return size of the new vectors
}

vec_basic MultivariatePolynomial::get_args() const
{
    vec_basic args;
    umap_uvec_expr d;
    // To change the ordering in which the terms appear in the vector, use
    // a different comparator for order_umap
    std::vector<vec_int> v = sorted_keys(dict_);
    for (const auto &p : v) {
        RCP<const Basic> res = ((dict_.find(p)->second).get_basic());
        int whichvar = 0;
        for (auto sym : vars_) {
            if (0 != p[whichvar])
                res = SymEngine::mul(res, pow(sym, integer(p[whichvar])));
            whichvar++;
        }
        args.push_back(res);
    }
    return args;
}

std::size_t MultivariatePolynomial::__hash__() const
{
    std::size_t seed = MULTIVARIATEPOLYNOMIAL;
    for (auto var : vars_)
        hash_combine<std::string>(seed, var->__str__());

    for (auto &p : dict_) {
        std::size_t t = vec_int_hash()(p.first);
        hash_combine<Basic>(t, *(p.second.get_basic()));
        seed ^= t;
    }
    return seed;
}

Expression MultivariatePolynomial::eval(
    std::map<RCP<const Basic>, Expression, RCPBasicKeyLess> &vals) const
{
    Expression ans(0);
    for (auto bucket : dict_) {
        Expression term = bucket.second;
        unsigned int whichvar = 0;
        for (auto sym : vars_) {
            term *= pow_ex(vals.find(sym)->second, bucket.first[whichvar]);
            whichvar++;
        }
        ans += term;
    }
    return ans;
}

RCP<const MultivariatePolynomial>
MultivariatePolynomial::convert(const UExprPoly &o)
{
    vec_basic s;
    s.push_back(o.get_var());

    umap_vec_expr d;
    for (auto &p : o.get_dict()) {
        vec_int v;
        v.push_back(p.first);
        d[v] = p.second;
    }
    return MultivariatePolynomial::create(s, std::move(d));
}

} // SymEngine
