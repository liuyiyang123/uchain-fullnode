/**
 * Copyright (c) 2011-2018 UChain developers (see AUTHORS)
 *
 * This file is part of uc-node.
 *
 * UChain is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef UC_CHAIN_ATTACHMENT_HPP
#define UC_CHAIN_ATTACHMENT_HPP

#include <cstdint>
#include <istream>
#include <UChain/bitcoin/define.hpp>
#include <UChain/bitcoin/utility/reader.hpp>
#include <UChain/bitcoin/utility/writer.hpp>
#include <boost/variant.hpp>
#include <UChain/bitcoin/chain/attachment/asset/asset.hpp>
#include <UChain/bitcoin/chain/attachment/asset/asset_cert.hpp>
#include <UChain/bitcoin/chain/attachment/asset/asset_mit.hpp>
#include <UChain/bitcoin/chain/attachment/asset/attenuation_model.hpp>
#include <UChain/bitcoin/chain/attachment/did/did.hpp>
#include <UChain/bitcoin/chain/attachment/ucn/ucn.hpp>
#include <UChain/bitcoin/chain/attachment/ucn/ucn_award.hpp>
#include <UChain/bitcoin/chain/attachment/message/message.hpp>

using namespace libbitcoin::chain;
#define TYPE2UINT32(kd)  (static_cast<typename std::underlying_type<attachment::attachment_type>::type>(kd))

#define UCN_TYPE        TYPE2UINT32(attachment::attachment_type::attachment_ucn)
#define UCN_AWARD_TYPE  TYPE2UINT32(attachment::attachment_type::attachment_ucn_award)
#define ASSET_TYPE      TYPE2UINT32(attachment::attachment_type::attachment_asset)
#define MESSAGE_TYPE    TYPE2UINT32(attachment::attachment_type::attachment_message)
#define DID_TYPE        TYPE2UINT32(attachment::attachment_type::attachment_did)
#define ASSET_CERT_TYPE TYPE2UINT32(attachment::attachment_type::attachment_asset_cert)
#define ASSET_MIT_TYPE  TYPE2UINT32(attachment::attachment_type::attachment_asset_mit)

#define DID_ATTACH_VERIFY_VERSION       TYPE2UINT32(207)


namespace libbitcoin {
namespace chain {

class BC_API attachment
{
public:

    enum class attachment_type : uint32_t
    {
        attachment_ucn, // ucn
        attachment_ucn_award,
        attachment_asset,
        attachment_message,
        attachment_did,
        attachment_asset_cert,
        attachment_asset_mit
    };

    typedef boost::variant<
        ucn,
        ucn_award,
        asset,
        blockchain_message,
        did,
        asset_cert,
        asset_mit
        > attachment_data_type;

    attachment();

    attachment(const std::string& from_did, const std::string& to_did);

    template<class Type>
    attachment(uint32_t type, uint32_t version, const Type& attach_data)
        : type(type), version(version), attach(attach_data)
    {}

    static attachment factory_from_data(const data_chunk& data);
    static attachment factory_from_data(std::istream& stream);
    static attachment factory_from_data(reader& source);
    static uint64_t satoshi_fixed_size();

    bool from_data(const data_chunk& data);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);
    data_chunk to_data() const;
    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;
    std::string to_string() const;
    bool is_valid() const;
    bool is_valid_type() const;
    void reset();
    uint64_t serialized_size() const;

    uint32_t get_version() const;
    void set_version(uint32_t version);
    uint32_t get_type() const;
    void set_type(uint32_t type);

    std::string get_to_did() const;
    void set_to_did(const std::string& did);

    std::string get_from_did() const;
    void set_from_did(const std::string& did);

    template<class Type>
    void set_attach(const Type& attach)
    {
         this->attach = attach;
    };
    attachment_data_type& get_attach();
    const attachment_data_type& get_attach() const;

private:
    uint32_t version;
    uint32_t type;
    std::string todid;
    std::string fromdid;
    attachment_data_type attach;
};

} // namespace chain
} // namespace libbitcoin

#endif
