#pragma once

#include "Core/Constexpr/ConstexprCollections.h"

namespace Constexpr {
    //A very basic (read: missing features, probably buggy; not simple) Regex library based on
    //the work here: https://swtch.com/~rsc/regexp/regexp1.html

    namespace Regex {
        namespace detail {
            static constexpr char Concat = '`';
            static constexpr int SplitCode = 256;
            static constexpr int MatchCode = 257;

            constexpr std::string ToPostfix(std::string_view sv) {
                std::string result;
                int atom = 0;
                int alt = 0;
                bool inRep = false;

                struct Group { int alt; int atom; };
                Constexpr::Stack<Group> groups;
                for (auto c : sv) {
                    switch (c) {
                    case '(': {
                        if (atom > 1) {
                            --atom;
                            result.push_back(Concat);
                        }
                        groups.push({ alt, atom });
                        atom = 0; alt = 0;
                        break;
                    }
                    case '|': {
                        if (atom == 0) return "";
                        while (--atom > 0) result.push_back(Concat);
                        alt++;
                        break;
                    }
                    case ')': {
                        if (groups.is_empty() || atom == 0) return "";
                        while (--atom > 0) result.push_back(Concat);
                        while (alt-- > 0) result.push_back('|');
                        auto group = groups.pop();
                        alt = group.alt;
                        atom = group.atom + 1;
                        break;
                    }
                    case '*':
                    case '+':
                    case '?':
                        if (atom == 0) return "";
                        result.push_back(c);
                        break;
                    case '{':
                        inRep = true;
                        result.push_back('{'); //keep the curly, but don't count it as an atom
                        break;
                    case '}':
                        if (!inRep) return "";
                        inRep = false;
                        result.push_back('}');
                        break;
                    case Concat:
                        return "";
                    case ',':
                        if (inRep) {
                            result.push_back(10); //signal between {3} and {3,}
                            break;
                        }
                        //fallthrough
                    case'1': case'2': case'3': case '4': case '5': case '6': case '7': case '8': case'9':
                        if (inRep) {
                            result.push_back(c - '0');
                            break;
                        }
                        //fallthrough
                    default:
                        if (atom > 1) {
                            --atom;
                            result.push_back(Concat);
                        }
                        result.push_back(c);
                        atom++;
                        break;
                    }
                }
                if (!groups.is_empty()) return "";
                if (inRep) return "";
                while (--atom > 0) result.push_back(Concat);
                while (alt-- > 0) result.push_back('|');

                return result;
            }
        }
        class Re {
        private:
            static const size_t Unset = 99'999'999;

        public:
            constexpr Re(std::string_view sv) {
                Nodes.reserve(10'000); //brittle.  Try to prevent vector from resizing while adding nodes
                ToRegex(detail::ToPostfix(sv));
            }
            constexpr bool Matches(std::string_view sv) {
                std::vector<size_t> current;
                std::vector<size_t> next;

                AddNext(current, StartingNode);
                for (auto c : sv) {
                    for (auto nodeIndex : current) {
                        const auto& node = Nodes[nodeIndex];
                        if (node.code == c) {
                            AddNext(next, node.out1);
                        }
                    }
                    current = next;
                    next.clear();
                    if (current.empty()) return false;
                }
                return std::any_of(current.begin(), current.end(), [&](size_t nodeId) { return Nodes[nodeId].code == detail::MatchCode; });
            }

        private:
            struct Node {
                int code{ 0 };
                size_t out1{ Unset };
                size_t out2{ Unset };
            };

            std::vector<Node> Nodes;
            size_t StartingNode{ 0 };

            constexpr void AddNext(std::vector<size_t>& next, size_t current) {
                if (Nodes[current].code == detail::SplitCode) {
                    AddNext(next, Nodes[current].out1);
                    AddNext(next, Nodes[current].out2);
                }
                else {
                    next.push_back(current);
                }
            }
            constexpr void Patch(const std::vector<size_t*>& outs, size_t value) {
                for (auto out : outs) {
                    *out = value;
                }
            }

            constexpr void ToRegex(const std::string& postfix) {
                if (postfix.empty()) return;
                struct Frag { size_t nodeId{ 0 }; std::vector<size_t*> outs; };
                Constexpr::Stack<Frag> stack;
                Frag e1, e2;
                Node node;
                size_t nodeId = Nodes.size();
                bool inRep = false;
                int minRep = 0;
                int maxRep = 0;

                for (auto c : postfix) {
                    nodeId = Nodes.size();
                    switch (c) {
                    case detail::Concat: {
                        e2 = stack.pop();
                        e1 = stack.pop();
                        Patch(e1.outs, e2.nodeId);
                        stack.push({ e1.nodeId, e2.outs });
                        break;
                    }
                    case '|': {	// alternate
                        e2 = stack.pop();
                        e1 = stack.pop();
                        
                        node = { detail::SplitCode, e1.nodeId, e2.nodeId };
                        std::copy(e2.outs.begin(), e2.outs.end(), std::back_inserter(e1.outs));
                        Nodes.push_back(node);
                        stack.push({ nodeId, e1.outs });
                        
                        break;
                    }
                    case '?': {	// zero or one
                        e1 = stack.pop();
                        node = { detail::SplitCode, e1.nodeId, Unset };
                        Nodes.push_back(node);
                        e1.outs.push_back(&Nodes.back().out2);
                        stack.push({ nodeId, e1.outs });
                        break;
                    }
                    case '*': {	// zero or more
                        e1 = stack.pop();
                        node = { detail::SplitCode, e1.nodeId, Unset };
                        Patch(e1.outs, nodeId);
                        Nodes.push_back(node);
                        stack.push({ nodeId, {&Nodes.back().out2} });
                        break;
                    }
                    case '+': {	// one or more 
                        e1 = stack.pop();
                        node = { detail::SplitCode, e1.nodeId, Unset };
                        Nodes.push_back(node);
                        Patch(e1.outs, nodeId);
                        stack.push({ e1.nodeId, {&Nodes.back().out2} });
                        break;
                    }
                    case '{': //start of grouping
                        inRep = true;
                        minRep = 0;
                        maxRep = 0;
                        break;
                    case '}': {
                        if (!inRep) throw "Invalid regex";
                        inRep = false;

                        e1 = stack.pop(); //b{3}
                        Patch(e1.outs, nodeId);

                        const Node toDuplicate = Nodes.back();
                        for (size_t i = 0; i < minRep - 1; i++) {
                            Nodes.push_back(toDuplicate);
                            Nodes.back().out1 += i + 1;
                        }
                        if (maxRep > minRep) {
                            for (size_t i = minRep; i < maxRep; i++) {
                                //e{3,5} == eeee?e?
                                Nodes.push_back({ detail::SplitCode, Nodes.size() + 1, Nodes.size() + 2 });
                                Nodes.push_back(toDuplicate);
                                Nodes.back().out1 += i + 2;
                            }
                            stack.push({ e1.nodeId, {&Nodes.back().out2} });
                        }
                        else {
                            if (maxRep == -1) {
                                //e{3,} == eee+
                                Nodes.push_back({ detail::SplitCode, Nodes.size() - 1, Unset });
                                stack.push({ e1.nodeId, {&Nodes.back().out2} });
                            }
                            else {
                                stack.push({ e1.nodeId, {&Nodes.back().out1} });
                            }
                        }
                        break;
                    }
                    case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:
                        minRep == 0 ? minRep = c : maxRep = c;
                        break;
                    case 10:
                        maxRep = -1;
                        break;
                    default: {
                        node = { c, Unset, Unset };
                        Nodes.push_back(node);
                        stack.push({ nodeId, {&Nodes.back().out1} });
                        break;
                    }
                    }

                }

                e1 = stack.pop();
                node = { detail::MatchCode, Unset, Unset };
                Patch(e1.outs, Nodes.size());
                Nodes.push_back(node);
                StartingNode = e1.nodeId;
            }
        };

        constexpr bool Test(const std::string& regex, const std::string& toMatch) {
            return Re(regex).Matches(toMatch);
        }

        constexpr bool RunTest() {
            static_assert(Test("abc", "abc"));
            static_assert(Test("a(b|c)", "ab"));
            static_assert(Test("a(b|c)", "ac"));
            static_assert(!Test("a(b|c)", "abc"));

            static_assert(Test("a*", "aaaa"));
            static_assert(Test("ab*c", "ac"));
            static_assert(Test("a*b", "b")); //doesn't work
            static_assert(Test("a+", "aaaa"));
            static_assert(!Test("ab+", "a"));
            static_assert(Test("a|b", "b"));
            //if (!Test("a|b", "b")) return false;

            static_assert(Test("a((aa|bb)(ab|ba)|(ab|ba)(aa|bb))b", "abbbab"));
            static_assert(Test("a((aa|bb)(ab|ba)|(ab|ba)(aa|bb))b", "ababbb"));
            //if(!Test("a((aa|bb)(ab|ba)|(ab|ba)(aa|bb))b", "abbbab")) return false;

            static_assert(Test("ab{3}", "abbb"));
            static_assert(Test("ab{3,5}", "abbbb"));
            static_assert(Test("ab{3,}", "abbbbbb"));

            return true;
        }
    }
}