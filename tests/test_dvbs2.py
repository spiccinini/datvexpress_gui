import unittest

from pydvbs2.clib import ffi, lib

class DVBS2TestCase(unittest.TestCase):
    def setUp(self):
        self.encoder = ffi.new("dvbs2_t **")
        self.encoder = lib.dvbs2_new()
    def tearDown(self):
        lib.dvbs2_free(self.encoder)

    def test_configure(self):
        status = lib.dvbs2_config(self.encoder, lib.DVBS2_CR_1_2, lib.DVBS2_M_QPSK)
        self.assertEqual(status, 0)

        status = lib.dvbs2_config(self.encoder, lib.DVBS2_CR_1_2, lib.DVBS2_M_8PSK)
        self.assertEqual(status, -1)

    def test_bch_encode(self):
        lib.dvbs2_config(self.encoder, lib.DVBS2_CR_1_2, lib.DVBS2_M_QPSK)
        frame = ffi.new("uint32_t [%d]" % lib.FRAME_SIZE_NORMAL, [1,0]*(32400//2))
        encoded_bytes = lib.dvbs2_bch_encode(self.encoder, frame)
        self.assertEqual(encoded_bytes, 32400)
        expected = [0, 1, 1, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0,
                    1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1,
                    1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0,
                    1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1,
                    1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 1, 1, 1, 0]
        self.assertSequenceEqual(list(frame[32400-192:32400]), expected)



if __name__ == "__main__":
    unittest.main()
