#
# Copyright (c) 2023, Renesas Electronics Corporation. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice,
# this list of conditions and the following disclaimer in the documentation
# and/or other materials provided with the distribution.
#
# Neither the name of ARM nor the names of its contributors may be used
# to endorse or promote products derived from this software without specific
# prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#

ifneq (${TSIP_LIB_PATH},)

ifeq (${ENC_UFPK_BIN},)
  $(error Error: ENC_UFPK_BIN not set)
endif

SRC_FILE	+=	security/secprv.c security/ufpk.c

SRC_FILE	+=	security/tsip/wrapper/TSIP_Wrapper_Init_API.c		\
				security/tsip/wrapper/TSIP_Wrapper_Prepare_API.c	\
				security/tsip/wrapper/TSIP_Wrapper_Local_API.c		\
				security/tsip/core/TSIP_Core_Init_API.c				\
				security/tsip/core/TSIP_Core_Prepare_API.c			\
				security/tsip/core/TSIP_Core_Local_API.c			\
				security/tsip/core/TSIP_Core_Proc_API.c				\
				security/tsip/stub/TSIP_Driver_nonos.c				\
				security/tsip/stub/TSIP_Stub_API_nonos.c

CFLAGS		+=	-Isecurity -Isecurity/tsip -Isecurity/tsip/common/ -Isecurity/tsip/stub/

CFLAGS		+=	-DSECPROV=1 -D_TSIP_SB -D'ENC_UFPK_BIN="${ENC_UFPK_BIN}"'

LIBS		+=	${TSIP_LIB_PATH}

endif
