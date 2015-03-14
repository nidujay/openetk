#ifndef DJETK_MESSAGE_H
#define DJETK_MESSAGE_H

#include <cstdint>
#include <cstddef>

namespace djetk {

/**
 * \brief Base Message object
 */
struct Message {
    /**
     * \brief Default constructor. Undefined message
     */
    Message()
        : id(0), payload(static_cast<size_t>(0)) {}

    /**
     * \brief Construct a message with a non-pointer generic data member
     * \param[in]   id_arg  Message ID
     * \param[in]   data    Message specific data payload
     */
    Message(uint32_t id_arg, size_t data)
        : id(id_arg),
        payload(data) {}

    /**
     * \brief Construct a message with pointer to data
     * \param[in]   id_arg  Message ID
     * \param[in]   data    Pointer to application specific data object
     *
     * The ownership of the data is application specific.
     */
    Message(uint32_t id_arg, const void *data)
        : id(id_arg),
        payload(data) {}

    /**
     * \brief Message ID.
     */
    uint32_t id;
    /**
     * \brief Payload associated with the message.
     * This property can be a pointer or a reference to a message specific data object.
     * If using as a pointer, the ownership of the payload data is application specific.
     */
    union Payload {
        /**
         * \brief Integral data type constructor
         * \param[in]   data_arg    Integral payload data
         */
        explicit Payload(size_t data_arg)
            : data(data_arg) {}

        /**
         * \brief Pointer data type constructor
         * \param[in]   pdata_arg   Pointer to data
         */
        explicit Payload(const void *pdata_arg)
            : pdata(pdata_arg) {}

        /**
         * \brief Integral data
         */
        size_t data;

        /**
         * \brief Pointer data
         */
        const void *pdata;
    };

    /**
     * \brief Message payload;
     */
    Payload payload;
};

}   // namespace djetk

#endif

