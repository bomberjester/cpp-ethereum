/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Sealer.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 *
 * Determines the PoW algorithm.
 */

#pragma once

#include <functional>
#include <libdevcore/Guards.h>
#include "Common.h"

namespace dev
{
namespace eth
{

class BlockInfo;

class SealFace
{
public:
	virtual bool wouldSealHeader(BlockInfo const&) const { return true; }
	virtual bytes sealedHeader(BlockInfo const& _bi) const = 0;
};

class SealEngineFace
{
public:
	bytes option(std::string const& _name) const { Guard l(x_options); return m_options.count(_name) ? m_options.at(_name) : bytes(); }
	bool setOption(std::string const& _name, bytes const& _value) { Guard l(x_options); try { if (onOptionChanging(_name, _value)) { m_options[_name] = _value; return true; } } catch (...) {} return false; }

	virtual strings sealers() const { return { "default" }; }
	virtual void setSealer(std::string const&) {}
	virtual void generateSeal(BlockInfo const& _bi) = 0;
	virtual void onSealGenerated(std::function<void(SealFace const* s)> const& _f) = 0;
	virtual void disable() {}

	// TODO: rename & generalise
	virtual bool isMining() const { return false; }
	virtual MiningProgress miningProgress() const { return MiningProgress(); }

protected:
	virtual bool onOptionChanging(std::string const&, bytes const&) { return true; }
	void injectOption(std::string const& _name, bytes const& _value) { Guard l(x_options); m_options[_name] = _value; }

private:
	mutable Mutex x_options;
	std::unordered_map<std::string, bytes> m_options;
};

}
}
