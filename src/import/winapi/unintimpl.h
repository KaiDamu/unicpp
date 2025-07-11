#pragma once

dfa UNICODE_STRING_::UNICODE_STRING_()
{
}
dfa UNICODE_STRING_::UNICODE_STRING_(cx CH* Buffer)
{
    tx->Length = StrLen(Buffer) * siz(Buffer[0]);
    tx->MaximumLength = tx->Length + siz(Buffer[0]);
    tx->Buffer = TO(tx->Buffer)(Buffer);
}
